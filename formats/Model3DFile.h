#pragma once
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

// .3d files
class Model3DFile final
{
public:
    struct Vec2
    {
        float x, y;
    };

    struct Vec3
    {
        float x, y, z;
    };

    struct Frame
    {
        std::vector<Vec3> positions;
        std::vector<Vec3> normals;
    };

    struct Triangle
    {
        // these are the indices for the positions
        uint32_t v[3];
    };

    struct TriangleAttributes
    {
        uint32_t flags; // not sure about this one
        uint8_t colour[4]; // BGRA
        int32_t textureIndex;

        Vec2 texCoords[3]; // per-vertex
    };

    struct Vertex
    {
        Vec3 pos;
        Vec3 nor;
        Vec3 col;
        Vec2 tex;
        int32_t texIndex;
    };

    bool load(const uint8_t *data, size_t size);

    const unsigned getNumFrames() const {return frames.size();}
    const Frame &getFrame(unsigned index) const;

    const unsigned getNumTriangles() const {return triangles.size();}
    const Triangle &getTriangle(unsigned index) const;

    const TriangleAttributes &getTriangleAttributes(unsigned index) const;

    // helper
    Vertex getVertex(unsigned frame, unsigned index) const;

private:
    std::string name;
    std::vector<Frame> frames;
    std::vector<Triangle> triangles;
    std::vector<TriangleAttributes> triAttribs;

    // this number of triangles have per-vertex normals, the rest have per-face normals
    uint32_t numSmoothTriangles;
};