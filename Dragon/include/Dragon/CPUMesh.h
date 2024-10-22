#pragma once

#include <string>
#include <memory>
#include <span>

#include <Dragon/Mesh.h>

namespace Dragon
{
    struct CPUMesh
    {
    public:
        static CPUMesh LoadFromFile(const std::string& path);
        static CPUMesh Cube();
    public:
        CPUMesh(unsigned int vertex_count, unsigned int index_count);
        ~CPUMesh() = default;
        CPUMesh(const CPUMesh&) = delete;
        CPUMesh(CPUMesh&&) noexcept = default;
        CPUMesh& operator=(const CPUMesh&) = delete;
        CPUMesh& operator=(CPUMesh&&) noexcept = default;
    public:
        unsigned int GetVertexCount() const { return m_vertex_count; }
        unsigned int GetIndexCount() const { return m_index_count; }
    public:
        float* GetRawVertexData(int i) { return m_vertex_buffers[i].get(); }
        float* GetRawVertexData(Mesh::VertexBufferIdx idx) { return GetRawVertexData(static_cast<int>(idx)); }
        const float* GetRawVertexData(int i) const { return m_vertex_buffers[i].get(); }
        const float* GetRawVertexData(Mesh::VertexBufferIdx idx) const { return GetRawVertexData(static_cast<int>(idx)); }
    public:
        float* GetRawPositions() { return m_vertex_buffers[static_cast<int>(Mesh::VertexBufferIdx::Positions)].get(); }
        float* GetRawNormals() { return m_vertex_buffers[static_cast<int>(Mesh::VertexBufferIdx::Normals)].get(); }
        float* GetRawUVs() { return m_vertex_buffers[static_cast<int>(Mesh::VertexBufferIdx::UVs)].get(); }
        const float* GetRawPositions() const { return m_vertex_buffers[static_cast<int>(Mesh::VertexBufferIdx::Positions)].get(); }
        const float* GetRawNormals() const { return m_vertex_buffers[static_cast<int>(Mesh::VertexBufferIdx::Normals)].get(); }
        const float* GetRawUVs() const { return m_vertex_buffers[static_cast<int>(Mesh::VertexBufferIdx::UVs)].get(); }
        std::span<Mesh::VertexPosition> GetPositions() { return { reinterpret_cast<Mesh::VertexPosition*>(GetRawPositions()), m_vertex_count }; }
        std::span<Mesh::VertexNormal> GetNormals() { return { reinterpret_cast<Mesh::VertexNormal*>(GetRawNormals()), m_vertex_count }; }
        std::span<Mesh::VertexUV> GetUVs() { return { reinterpret_cast<Mesh::VertexUV*>(GetRawUVs()), m_vertex_count }; }
        std::span<const Mesh::VertexPosition> GetPositions() const { return { reinterpret_cast<const Mesh::VertexPosition*>(GetRawPositions()), m_vertex_count }; }
        std::span<const Mesh::VertexNormal> GetNormals() const { return { reinterpret_cast<const Mesh::VertexNormal*>(GetRawNormals()), m_vertex_count }; }
        std::span<const Mesh::VertexUV> GetUVs() const { return { reinterpret_cast<const Mesh::VertexUV*>(GetRawUVs()), m_vertex_count }; }
    public:
        uint32_t* GetRawIndices() { return m_indices.get(); }
        std::span<uint32_t> GetIndices() { return { GetRawIndices(), m_index_count }; }
        const uint32_t* GetRawIndices() const { return m_indices.get(); }
        std::span<const uint32_t> GetIndices() const { return { GetRawIndices(), m_index_count }; }
    private:
        unsigned int m_vertex_count;
        unsigned int m_index_count;
        std::unique_ptr<float[]> m_vertex_buffers[static_cast<int>(Mesh::VertexBufferIdx::Count)];
        std::unique_ptr<uint32_t[]> m_indices;
    };
}