#include "CRDFile.h"

bool CRDFile::load(const uint8_t *data, size_t size)
{
    if(size < 920)
        return false;

    if(data[0] != 0x66)
        return false;

    id = *reinterpret_cast<const uint32_t *>(data + 8);

    toName = reinterpret_cast<const char *>(data + 12);
    fromName = reinterpret_cast<const char *>(data + 33);

    // there is some unknown data in the region above

    // background colour
    backgroundCol[0] = data[60];
    backgroundCol[1] = data[61];
    backgroundCol[2] = data[62];

    // max 80 chars
    message = reinterpret_cast<const char *>(data + 63);

    // stamp/mark
    stampIndex = data[143];
    markIndex[0] = data[144];
    markIndex[1] = data[145];

    // clipart
    auto ptr = data + 146;

    for(int i = 0; i < 128; i++)
    {
        auto idHigh = *ptr++;

        clipart[i].index = *ptr++;
        clipart[i].category = idHigh >> 3;
        clipart[i].subCategory = idHigh & 7;

        clipart[i].x = *ptr++;
        clipart[i].w = *ptr++;
        clipart[i].y = *ptr++;
        clipart[i].h = *ptr++;
    }

    return true;
}

void CRDFile::getBackgroundColourRGB(uint8_t rgb[3]) const
{
    auto r = backgroundCol[0];
    auto y = backgroundCol[1];
    auto b = backgroundCol[2];

    // the game has a very basic RYB->RGB conversion
    int out_r = 255 + y / 4 - b     + r / 3;
    int out_g = 255 + y / 2 - b / 2 - r    ;
    int out_b = 255 - y     + b / 3 - r    ;

    out_r = std::min(std::max(out_r, 0), 255);
    out_g = std::min(std::max(out_g, 0), 255);
    out_b = std::min(std::max(out_b, 0), 255);

    rgb[0] = out_r;
    rgb[1] = out_g;
    rgb[2] = out_b;
}

std::string CRDFile::getStampFilename() const
{
    if(stampIndex == 0)
        return "";

    char buf[10];
    snprintf(buf, 10, "s%04i.bmp", stampIndex);

    return buf;
}

std::string CRDFile::getMarkFilename() const
{
    if(markIndex[0] == 0 || markIndex[1] == 0)
        return "";

    // mark[0] == 1, [1] == 1 -> R0001
    //     [0] == 2, [1] == 1 -> R0002
    //     [0] == 1, [1] == 2 -> R1001

    char buf[10];
    snprintf(buf, 10, "r%i%03i.bmp", markIndex[1] - 1, markIndex[0]);
    return buf;
}

std::string CRDFile::getClipartFilename(unsigned index) const
{
    if(index >= 128)
        return "";

    auto &clipartImage = getClipart(index);

    // empty entry
    if(clipartImage.index == 0)
        return "";

    // map category to prefix char
    char categoryChar = 0;
    
    if(clipartImage.category < 10) // 0-9
        categoryChar = '0' + clipartImage.category;
    else if(clipartImage.category < 16) // A-F
        categoryChar = 'a' + clipartImage.category - 10;
    else if(clipartImage.category == 16) // H (there is no G)
        categoryChar = 'h';
    else
        return "";

    char buf[10];
    snprintf(buf, 10, "%c%i%03i.bmp", categoryChar, clipartImage.subCategory, clipartImage.index);
    return buf;
}