#include <Dragon/pch.h>
#include <Dragon/TextureMgr.h>
#include <Dragon/Commons.h>

namespace Dragon
{
    TextureMgr::TextureMgr(ID3D11Device* device)
        : m_device{ device }
        , m_cells{}
        , m_texture_by_name{}
    {
    }

    TextureRef TextureMgr::Load(const std::filesystem::path& path)
    {
        Dragon_Check(path.has_filename());

        auto texture_name{ path.filename().stem().string() };
        if (auto it{ m_texture_by_name.find(texture_name) }; it != m_texture_by_name.end())
        {
            auto cell{ std::find_if(m_cells.begin(), m_cells.end(), [=](const auto& cell) { return cell.texture.get() == it->second; }) };
            Dragon_Check(cell != m_cells.end());
            (*cell->ref_count)++;
            return { cell->ref_count.get(), cell->texture.get() };
        }
        else
        {
            auto& cell{ m_cells.emplace_back() };
            cell.ref_count = std::make_unique<size_t>(1);
            cell.texture = std::make_unique<Texture>(m_device, path);
            m_texture_by_name.emplace(path.filename().stem().string(), cell.texture.get());
            return { cell.ref_count.get(), cell.texture.get() };
        }
    }
}