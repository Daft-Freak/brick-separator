#pragma once
#include <cstddef>
#include <cstdint>

class LLSFile final
{
public:
    ~LLSFile();

    bool load(const uint8_t *data, size_t size);

    uint32_t getWidth() const;
    uint32_t getHeight() const;
    int getBitsPerPixel() const;
    uint32_t getNumFrames() const;
    uint32_t getFlags() const;

    // buf should be w*h*4 bytes
    bool decodeFrame(uint32_t frame, uint8_t *buf);

private:
    bool doDecodeFrame(uint32_t frame, uint8_t *buf);

    uint8_t *data = nullptr;

    uint8_t **frameBase = nullptr;
};