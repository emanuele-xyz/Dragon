#pragma once

#include <Dragon/D3D11Utils.h>

#include <filesystem>

#define DRAGON_MESH_FLOATS_PER_POSITION 3
#define DRAGON_MESH_FLOATS_PER_NORMAL 3
#define DRAGON_MESH_FLOATS_PER_UV 2

namespace Dragon
{
    class Mesh
    {
    public:
        Mesh(ID3D11Device* device, const std::filesystem::path& path);
        Mesh(ID3D11Device* device, unsigned vertex_count, unsigned index_count, float* positions, float* normals, float* uvs, uint32_t* indices);
        ~Mesh() = default;
        Mesh(const Mesh&) = delete;
        Mesh(Mesh&&) noexcept = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&) noexcept = delete;
    public:
        unsigned GetVertexBufferCount() { return 3; }
        unsigned* GetStrides() { return m_strides; }
        unsigned* GetOffsets() { return m_offsets; }
        unsigned GetIndexCount() { return m_index_count; }
        ID3D11Buffer** GetVertexBuffers() { return m_vertex_buffers; }
        ID3D11Buffer* GetIndices() { return m_indices.Get(); }
    private:
        unsigned m_vertex_count;
        unsigned m_index_count;
        wrl::ComPtr<ID3D11Buffer> m_positions;
        wrl::ComPtr<ID3D11Buffer> m_normals;
        wrl::ComPtr<ID3D11Buffer> m_uvs;
        wrl::ComPtr<ID3D11Buffer> m_indices;
        ID3D11Buffer* m_vertex_buffers[3];
        unsigned m_strides[3];
        unsigned m_offsets[3];
    };
}
