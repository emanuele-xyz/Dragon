#pragma once

#include <Dragon/D3D11Utils.h>

#define DRAGON_MESH_FLOATS_PER_POSITION 3
#define DRAGON_MESH_FLOATS_PER_NORMAL 3
#define DRAGON_MESH_FLOATS_PER_UV 2

namespace Dragon
{
    class Mesh
    {
    public:
        Mesh(ID3D11Device* device, const std::string& path);
        ~Mesh() = default;
        Mesh(const Mesh&) = delete;
        Mesh(Mesh&&) noexcept = default;
        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&) noexcept = default;
    public:
        unsigned GetVertexBufferCount() { return 3; }
        unsigned* GetStrides() { return m_strides; }
        unsigned* GetOffsets() { return m_offsets; }
        unsigned GetIndexCount() { return m_index_count; }
        ID3D11Buffer** GetVertexBuffers() { return m_vertex_buffers; }
        ID3D11Buffer* GetIndices() { return m_indices.Get(); }
    private:
        unsigned int m_vertex_count;
        unsigned int m_index_count;
        wrl::ComPtr<ID3D11Buffer> m_positions;
        wrl::ComPtr<ID3D11Buffer> m_normals;
        wrl::ComPtr<ID3D11Buffer> m_uvs;
        wrl::ComPtr<ID3D11Buffer> m_indices;
        ID3D11Buffer* m_vertex_buffers[3];
        unsigned m_strides[3];
        unsigned m_offsets[3];
    };
}
