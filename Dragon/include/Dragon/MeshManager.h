#pragma once

#include <Dragon/GPUMesh.h>

#include <vector>
#include <string>
#include <unordered_map>

namespace Dragon
{
    class MeshManager
    {
    public:
        MeshManager() = default;
        ~MeshManager() = default;
        MeshManager(const MeshManager&) = delete;
        MeshManager(MeshManager&&) noexcept = delete;
        MeshManager& operator=(const MeshManager&) = delete;
        MeshManager& operator=(MeshManager&&) noexcept = delete;
    public:
        size_t RegisterMesh(GPUMesh&& mesh, const std::string& name);
    private:
        std::vector<GPUMesh> m_meshes;
        std::unordered_map<std::string, size_t> m_meshes_by_name;
    };
}
