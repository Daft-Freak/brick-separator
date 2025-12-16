#include "CSPFile.h"

struct CSPHeader
{
    uint16_t numSprites;
    uint16_t unk; // usually 2? 0 if numSprites is 0
};

bool CSPFile::load(const uint8_t *data, size_t size)
{
    if(size < 8)
        return false;

    auto ptr = data;

    // string read helper
    auto readString = [&ptr]()
    {
        uint32_t len = *reinterpret_cast<const uint32_t *>(ptr);

        ptr += 4;

        auto ret = std::string_view(reinterpret_cast<const char *>(ptr), len);
        ptr += len;
        
        return ret;
    };

    // get header
    auto header = reinterpret_cast<const CSPHeader *>(ptr);
    ptr += sizeof(CSPHeader);

    name = readString();

    // read sprites
    sprites.resize(header->numSprites);

    for(unsigned i = 0; i < header->numSprites; i++)
    {
        sprites[i].x = *reinterpret_cast<const int32_t *>(ptr);
        ptr += 4;
        sprites[i].y = *reinterpret_cast<const int32_t *>(ptr);
        ptr += 4;
    }

    for(unsigned i = 0; i < header->numSprites; i++)
        sprites[i].name = readString();

    return true;
}

const CSPFile::Sprite &CSPFile::getSprite(unsigned index) const
{
    // should really validate this...
    return sprites[index];
}