#pragma once

#include <Dragon/TextureRef.h>

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <filesystem>

namespace Dragon
{
    class TextureMgr
    {
    public:
        TextureMgr(ID3D11Device* device);
        ~TextureMgr() = default;
        TextureMgr(const TextureMgr&) = delete;
        TextureMgr(TextureMgr&&) noexcept = delete;
        TextureMgr& operator=(const TextureMgr&) = delete;
        TextureMgr& operator=(TextureMgr&&) noexcept = delete;
    public:
        TextureRef Load(const std::filesystem::path& path);
    private:
        struct Cell
        {
            std::unique_ptr<size_t> ref_count;
            std::unique_ptr<Texture> texture;
        };
    private:
        ID3D11Device* m_device;
        std::vector<Cell> m_cells;
        std::unordered_map<std::string, Texture*> m_texture_by_name;
    };
}
