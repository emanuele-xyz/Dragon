#include <Dragon/pch.h>
#include <Dragon/GPUMesh.h>

namespace Dragon
{
    GPUMesh GPUMesh::FromCPUMesh(ID3D11Device* device, const CPUMesh& cpu_mesh)
    {
        GPUMesh mesh{};
        mesh.vertex_count = cpu_mesh.GetVertexCount();
        mesh.index_count = cpu_mesh.GetIndexCount();

        // NOTE: vertex buffers
        for (int i{}; i < static_cast<int>(Mesh::VertexBufferIdx::Count); i++)
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = cpu_mesh.GetVertexCount() * Mesh::GetFloatsPerVertex(i) * sizeof(float);
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA subres_data{};
            subres_data.pSysMem = cpu_mesh.GetRawVertexData(i);
            subres_data.SysMemPitch = 0;
            subres_data.SysMemSlicePitch = 0;

            mesh.vertex_buffers[i] = D3D11Utils::CreateBuffer(device, &desc, &subres_data);
            mesh.vertex_buffer_pointers[i] = mesh.vertex_buffers[i].Get();
            mesh.vertex_buffer_strides[i] = Mesh::GetFloatsPerVertex(i) * sizeof(float);
            mesh.vertex_buffer_offsets[i] = 0;
        }

        // NOTE: index buffer
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = cpu_mesh.GetIndexCount() * sizeof(uint32_t);
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA subres_data{};
            subres_data.pSysMem = cpu_mesh.GetRawIndices();
            subres_data.SysMemPitch = 0;
            subres_data.SysMemSlicePitch = 0;

            mesh.indices = D3D11Utils::CreateBuffer(device, &desc, &subres_data);
        }

        return mesh;
    }
}