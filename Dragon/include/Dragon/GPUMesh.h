#pragma once

#include <Dragon/D3D11Utils.h>
#include <Dragon/CPUMesh.h>

namespace Dragon
{
    class GPUMesh
    {
    public:
        static GPUMesh FromCPUMesh(ID3D11Device* device, const CPUMesh& cpu_mesh);
    public:
        GPUMesh() = default;
        ~GPUMesh() = default;
        GPUMesh(const GPUMesh&) = delete;
        GPUMesh(GPUMesh&&) noexcept = default;
        GPUMesh& operator=(const GPUMesh&) = delete;
        GPUMesh& operator=(GPUMesh&&) noexcept = default;
    public:
        UINT vertex_count;
        UINT index_count;
        wrl::ComPtr<ID3D11Buffer> vertex_buffers[static_cast<int>(Mesh::VertexBufferIdx::Count)];
        wrl::ComPtr<ID3D11Buffer> indices;
        ID3D11Buffer* vertex_buffer_pointers[static_cast<int>(Mesh::VertexBufferIdx::Count)];
        UINT vertex_buffer_strides[static_cast<int>(Mesh::VertexBufferIdx::Count)];
        UINT vertex_buffer_offsets[static_cast<int>(Mesh::VertexBufferIdx::Count)];
    };
}
