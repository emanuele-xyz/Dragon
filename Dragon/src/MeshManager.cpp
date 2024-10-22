#include <Dragon/pch.h>
#include <Dragon/MeshManager.h>

namespace Dragon
{
    size_t MeshManager::RegisterMesh(GPUMesh&& mesh, const std::string& name)
    {
        Dragon_Check(!m_meshes_by_name.contains(name));
        size_t idx{ m_meshes.size() };
        m_meshes.emplace_back(std::move(mesh));
        m_meshes_by_name.insert({ name, idx });
        return idx;
    }
}