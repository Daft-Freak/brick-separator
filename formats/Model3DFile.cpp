#include <cstring>

#include "Model3DFile.h"

static_assert(sizeof(Model3DFile::Vec3) == 12);
static_assert(sizeof(Model3DFile::Triangle) == 12);
static_assert(sizeof(Model3DFile::TriangleAttributes) == 36);

bool Model3DFile::load(const uint8_t *data, size_t size)
{
    if(size < 4)
        return false;

    auto ptr = data;
    auto endPtr = data + size;

    auto getU32 = [&ptr]()
    {
        auto ret = *reinterpret_cast<const uint32_t *>(ptr);
        ptr += 4;
        return ret;
    };

    auto numFrames = getU32();

    frames.clear();
    frames.resize(numFrames);
  
    // read frame position/normal data
    for(unsigned i = 0; i < numFrames; i++)
    {
        auto numPositions = getU32();
        frames[i].positions.resize(numPositions);

        // test.3d appears to have a bad frame, let's not crash
        if(ptr + numPositions * sizeof(Vec3) > endPtr)
            return false;

        memcpy(frames[i].positions.data(), ptr, numPositions * sizeof(Vec3));
        ptr += numPositions * sizeof(Vec3);

        auto numNormals = getU32();
        frames[i].normals.resize(numNormals);
        memcpy(frames[i].normals.data(), ptr, numNormals * sizeof(Vec3));
        ptr += numNormals * sizeof(Vec3);
    }

    // now we have the triangles
    auto numTriangles = getU32();
    numSmoothTriangles = getU32();

    triangles.clear();
    triangles.resize(numTriangles);
    memcpy(triangles.data(), ptr, numTriangles * sizeof(Triangle));
    ptr += numTriangles * sizeof(Triangle);

    // attribute data (tex coords/colours)
    triAttribs.clear();
    triAttribs.resize(numTriangles);
    memcpy(triAttribs.data(), ptr, numTriangles * sizeof(TriangleAttributes));

    return true;
}

const Model3DFile::Frame &Model3DFile::getFrame(unsigned index) const
{
    // should really validate this...
    return frames[index];
}

const Model3DFile::Triangle &Model3DFile::getTriangle(unsigned index) const
{
    return triangles[index];
}

const Model3DFile::TriangleAttributes &Model3DFile::getTriangleAttributes(unsigned index) const
{
    return triAttribs[index];
}

Model3DFile::Vertex Model3DFile::getVertex(unsigned frame, unsigned index) const
{
    if(frame >= getNumFrames() || index >= getNumTriangles() * 3)
        return {};

    auto &frameData = getFrame(frame);

    auto triangleIndex = index / 3;
    auto &triangle = getTriangle(triangleIndex);

    Vertex ret;
    ret.pos = frameData.positions[triangle.v[index % 3]];

    // get per-vertex or per-face normal
    if(triangleIndex < numSmoothTriangles)
        ret.nor = frameData.normals[index];
    else
    {
        auto perFaceNormalStart = numSmoothTriangles * 3;
        ret.nor = frameData.normals[perFaceNormalStart + triangleIndex - numSmoothTriangles];
    }

    // colour/texture
    auto &attribs = getTriangleAttributes(triangleIndex);
    if(attribs.textureIndex == -1)
    {
        // untextured
        ret.texIndex = -1;
        ret.tex = {0.0f, 0.0f};
        ret.col.x = attribs.colour[2] / 255.0f;
        ret.col.y = attribs.colour[1] / 255.0f;
        ret.col.z = attribs.colour[0] / 255.0f;
    }
    else
    {
        // textured
        ret.texIndex = attribs.textureIndex;
        ret.tex = attribs.texCoords[index % 3];
        ret.col = {1.0f, 1.0f, 1.0f};
    }

    return ret;
}