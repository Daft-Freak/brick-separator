#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

// loco cards
class CRDFile final
{
public:
    struct Clipart
    {
        uint8_t category;
        uint8_t subCategory;
        uint8_t index;

        uint8_t x, y, w, h;
    };

    bool load(const uint8_t *data, size_t size);

    uint32_t getID() const {return id;}

    const std::string &getToName() const {return toName;}
    const std::string &getFromName() const {return fromName;}

    const uint8_t *getBackgroundColour() const {return backgroundCol;}

    const std::string &getMessage() const {return message;}

    uint8_t getStampIndex() const {return stampIndex;}
    const uint8_t *getMarkIndex() const {return markIndex;}

    const Clipart &getClipart(unsigned index) const {return clipart[index];}

    // helpers
    void getBackgroundColourRGB(uint8_t rgb[3]) const;

    std::string getStampFilename() const;
    std::string getMarkFilename() const;

    std::string getClipartFilename(unsigned index) const;

private:
    uint32_t id;
    std::string toName, fromName;

    uint8_t backgroundCol[3]; // RYB

    std::string message;

    uint8_t stampIndex, markIndex[2];

    Clipart clipart[128];
};