#pragma once

namespace Dragon::Mesh
{
    struct VertexPosition
    {
        float x;
        float y;
        float z;
    };

    struct VertexNormal
    {
        float x;
        float y;
        float z;
    };

    struct VertexUV
    {
        float u;
        float v;
    };

    enum class VertexBufferIdx
    {
        Positions,
        Normals,
        UVs,
        Count
    };

    int GetFloatsPerVertex(VertexBufferIdx index);
    int GetFloatsPerVertex(int index);
}
