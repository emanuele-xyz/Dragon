#pragma once

#include <Dragon/MeshRef.h>

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <filesystem>

namespace Dragon
{
    class MeshMgr
    {
    public:
        MeshMgr(ID3D11Device* device);
        ~MeshMgr() {} /* = default*/; // TODO: use =default
        MeshMgr(const MeshMgr&) = delete;
        MeshMgr(MeshMgr&&) noexcept = delete;
        MeshMgr& operator=(const MeshMgr&) = delete;
        MeshMgr& operator=(MeshMgr&&) noexcept = delete;
    public:
        MeshRef Load(const std::filesystem::path& path);
    private:
        struct Cell
        {
            std::unique_ptr<size_t> ref_count;
            std::unique_ptr<Mesh> mesh;
        };
    private:
        ID3D11Device* m_device;
        std::vector<Cell> m_cells;
        std::unordered_map<std::string, Mesh*> m_mesh_by_name;
    };
}
