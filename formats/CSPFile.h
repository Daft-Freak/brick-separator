#pragma once
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

// composite sprite?
// ILF and TSF files seem to have the same format
class CSPFile final
{
public:
    struct Sprite
    {
        int32_t x, y;
        std::string name;
    };

    bool load(const uint8_t *data, size_t size);

    const std::string &getName() const {return name;}

    const unsigned getNumSprites() const {return sprites.size();}
    const Sprite &getSprite(unsigned index) const;

private:
    std::string name;
    std::vector<Sprite> sprites;
};