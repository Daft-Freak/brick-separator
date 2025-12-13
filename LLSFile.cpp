// presumably "LegoLand Sprite"
#include <cstring>

#include "LLSFile.h"

struct LLSHeader
{
    uint8_t magic[4]; // "COMP"
    uint32_t width;
    uint32_t height;
    uint32_t bpp;
    uint32_t numFrames;
    uint32_t flags;
};

// coincidentally, the header size in bytes is the same as the bits per pixel
struct LLSFrameHeader8
{
    uint32_t size; // size of entire frame (including header)
    uint32_t colourDataSize; // raw colour data (compressed)
};

struct LLSFrameHeader16
{
    uint32_t size;
    uint32_t colourDataSize; // x2 for bytes
    uint32_t lengthDataSize; // for compression
    uint32_t unknown;
};

static_assert(sizeof(LLSHeader) == 24);
static_assert(sizeof(LLSFrameHeader8) == 8);
static_assert(sizeof(LLSFrameHeader16) == 16);

enum LLSFlags
{
    LLS_Difference = 1 << 0, // frames are stored as diff from frame 0, which isn't displayed
    // 1 << 1 may be animated? or at least "has multiple frames" as there are some with one frame + the difference flag
};

LLSFile::~LLSFile()
{
    delete[] data;
    delete[] frameBase;
}

bool LLSFile::load(const uint8_t *data, size_t size)
{
    delete[] this->data;
    delete[] frameBase;
    frameBase = nullptr;

    // copy data
    this->data = new uint8_t[size];
    memcpy(this->data, data, size);

    auto header = reinterpret_cast<LLSHeader *>(this->data);

    // check magic
    if(memcmp(header->magic, "COMP", 4) != 0)
        return false;

    // TODO: more checks?


    auto numFrames = header->numFrames;

    // extra reference frame
    if(header->flags & LLS_Difference)
        numFrames++;

    // cache pointers to frames
    frameBase = new uint8_t *[numFrames];

    auto framePtr = this->data + sizeof(LLSHeader);
    for(unsigned i = 0; i < numFrames; i++)
    {
        frameBase[i] = framePtr;

        // doesn't matter which header we use, the size is the first in either
        auto frameHeader = reinterpret_cast<LLSFrameHeader8 *>(framePtr);
        framePtr += frameHeader->size;
    }

    return true;
}

uint32_t LLSFile::getWidth() const
{
    if(!data)
        return 0;

    auto header = reinterpret_cast<LLSHeader *>(data);
    return header->width;
}

uint32_t LLSFile::getHeight() const
{
    if(!data)
        return 0;

    auto header = reinterpret_cast<LLSHeader *>(data);
    return header->height;
}

int LLSFile::getBitsPerPixel() const
{
    if(!data)
        return 0;

    auto header = reinterpret_cast<LLSHeader *>(data);
    return header->bpp;
}

uint32_t LLSFile::getNumFrames() const
{
    if(!data)
        return 0;

    auto header = reinterpret_cast<LLSHeader *>(data);
    auto numFrames = header->numFrames;

    return numFrames;
}

uint32_t LLSFile::getFlags() const
{
    if(!data)
        return 0;

    auto header = reinterpret_cast<LLSHeader *>(data);
    return header->flags;
}

bool LLSFile::decodeFrame(uint32_t frame, uint8_t *buf)
{
    auto header = reinterpret_cast<LLSHeader *>(data);

    // validate
    if(frame >= getNumFrames())
        return false;

    // clear buffer
    memset(buf, 0, header->width * header->height * 4);

    if(header->flags & LLS_Difference)
    {
        // decode frame 0 first and offset frame num
        if(!doDecodeFrame(0, buf))
            return false;

        frame++;
    }

    return doDecodeFrame(frame, buf);
}

bool LLSFile::doDecodeFrame(uint32_t frame, uint8_t *buf)
{
    auto header = reinterpret_cast<LLSHeader *>(data);

    bool is8Bit = header->bpp == 8;
    auto width = header->width;
    auto height = header->height;

    // 555 -> 8888 conversion
    auto writePixel = [](uint8_t *&outPtr, uint16_t col)
    {
        uint8_t r = (col >> 10) & 0x1F;
        uint8_t g = (col >>  5) & 0x1F;
        uint8_t b = (col >>  0) & 0x1F;

        r = r << 3 | r >> 2;
        g = g << 3 | g >> 2;
        b = b << 3 | b >> 2;

        *outPtr++ = r;
        *outPtr++ = g;
        *outPtr++ = b;
        *outPtr++ = 0xFF;
    };

    // code bit reader
    int numCodeBits = 0;
    uint32_t curCodeWord = 0;
    uint32_t *codeData;

    auto getCodeBits = [&](int bits = 2)
    {
        // refill if not enough bits
        if(numCodeBits < bits)
        {
            curCodeWord = *codeData++;
            numCodeBits = 32;
        }

        numCodeBits -= bits;
        int ret = curCodeWord & ((1 << bits) - 1);
        curCodeWord >>= bits;

        return ret;
    };

    if(is8Bit)
    {
        // palette is only stored in first frame
        auto frame0Header = reinterpret_cast<LLSFrameHeader8 *>(frameBase[0]);
        auto palPtr = reinterpret_cast<uint16_t *>(frameBase[0] + sizeof(LLSFrameHeader8) + frame0Header->colourDataSize);

        auto framePtr = frameBase[frame];

        auto header = reinterpret_cast<LLSFrameHeader8 *>(framePtr);

        // compressed colour index data
        auto colData = framePtr + sizeof(LLSFrameHeader8);

        // decompression data
        // the lengths are encoded in the same data as the codes
        // this does result in some bits getting discarded sometimes
        codeData = reinterpret_cast<uint32_t *>(colData + header->colourDataSize);

        // skip palette for frame 0
        if(frame == 0)
            codeData += (256 * sizeof(uint16_t)) / sizeof(uint32_t);

        // data is compressed a line at a time
        for(unsigned y = 0; y < height; y++)
        {
            auto outPtr = buf + y * width * 4;

            while(true)
            {
                // raw copy until we reach a code 3
                while(true)
                {
                    auto code = getCodeBits();

                    if(code == 3) // done
                        break;

                    if(code == 2) // skip
                        outPtr += 4;
                    else // copy
                        writePixel(outPtr, palPtr[*colData++]);
                }

                // now with length
                auto count = getCodeBits(8);

                if(!count) // done
                    break;

                auto code = getCodeBits();

                if(code == 0) // copy
                {
                    for(unsigned i = 0; i < count; i++)
                        writePixel(outPtr, palPtr[*colData++]);
                }
                else if(code == 1) // fill
                {
                    auto col = palPtr[*colData++];
                    for(unsigned i = 0; i < count; i++)
                        writePixel(outPtr, col);
                }
                else // skip
                    outPtr += count * 4;
            }
        }
    }
    else
    {
        auto framePtr = frameBase[frame];

        auto header = reinterpret_cast<LLSFrameHeader16 *>(framePtr);

        // compressed 555 colour data
        auto colData = reinterpret_cast<uint16_t *>(framePtr + sizeof(LLSFrameHeader16));

        // decompression data
        auto lenData = reinterpret_cast<uint8_t *>(colData + header->colourDataSize);
        codeData = reinterpret_cast<uint32_t *>(lenData + header->lengthDataSize);

        // data is compressed a line at a time
        for(unsigned y = 0; y < height; y++)
        {
            auto outPtr = buf + y * width * 4;

            while(true)
            {
                // raw copy until we reach a code 3
                while(true)
                {
                    auto code = getCodeBits();

                    if(code == 3) // done
                        break;

                    if(code == 2) // skip
                        outPtr += 4;
                    else // copy
                        writePixel(outPtr, *colData++);
                }

                // now with length
                auto count = *lenData++;

                if(!count) // done
                    break;

                auto code = getCodeBits();

                if(code == 0) // copy
                {
                    for(unsigned i = 0; i < count; i++)
                        writePixel(outPtr, *colData++);
                }
                else if(code == 1) // fill
                {
                    auto col = *colData++;
                    for(unsigned i = 0; i < count; i++)
                        writePixel(outPtr, col);
                }
                else // skip
                    outPtr += count * 4;
            }
        }
    }

    return true;
}