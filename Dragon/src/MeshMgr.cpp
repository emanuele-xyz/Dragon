#include <Dragon/pch.h>
#include <Dragon/MeshMgr.h>

#include <Dragon/Commons.h>

namespace Dragon
{
    MeshMgr::MeshMgr(ID3D11Device* device)
        : m_device{ device }
        , m_cells{}
        , m_mesh_by_name{}
    {
    }

    MeshRef MeshMgr::Load(const std::filesystem::path& path)
    {
        Dragon_Check(path.has_filename());

        auto mesh_name{ path.filename().stem().string() };
        if (auto it{ m_mesh_by_name.find(mesh_name) }; it != m_mesh_by_name.end())
        {
            auto cell{ std::find_if(m_cells.begin(), m_cells.end(), [=](const auto& cell) { return cell.mesh.get() == it->second; }) };
            Dragon_Check(cell != m_cells.end());
            (*cell->ref_count)++;
            return { cell->ref_count.get(), cell->mesh.get() };
        }
        else
        {
            auto& cell{ m_cells.emplace_back() };
            cell.ref_count = std::make_unique<size_t>(1);
            cell.mesh = std::make_unique<Mesh>(m_device, path);
            m_mesh_by_name.emplace(path.filename().stem().string(), cell.mesh.get());
            return { cell.ref_count.get(), cell.mesh.get() };
        }
    }
}