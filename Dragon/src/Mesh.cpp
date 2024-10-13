#include <Dragon/pch.h>
#include <Dragon/Mesh.h>
#include <Dragon/Commons.h>

namespace Dragon::Mesh
{
    int GetFloatsPerVertex(VertexBufferIdx index)
    {
        switch (index)
        {
        case VertexBufferIdx::Positions: { return 3; } break;
        case VertexBufferIdx::Normals: { return 3; } break;
        case VertexBufferIdx::UVs: { return 2; } break;
        case VertexBufferIdx::Count:
        default: { Dragon_Unreachable(); } break;
        }
        return 0;
    }

    int GetFloatsPerVertex(int index)
    {
        return GetFloatsPerVertex(static_cast<VertexBufferIdx>(index));
    }
}