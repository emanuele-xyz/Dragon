#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <Dragon/GPUTexture.h>

namespace Dragon
{
    class TextureManager
    {
    public:
        TextureManager() = default;
        ~TextureManager() = default;
        TextureManager(const TextureManager&) = delete;
        TextureManager(TextureManager&&) noexcept = delete;
        TextureManager& operator=(const TextureManager&) = delete;
        TextureManager& operator=(TextureManager&&) noexcept = delete;
    public:
        size_t RegisterTexture(GPUTexture&& texture, const std::string& name);
        ID3D11ShaderResourceView* Get(size_t idx) { return m_textures[idx].GetSRV(); }
        ID3D11ShaderResourceView* Get(const std::string& name) { return m_textures[m_textures_by_name[name]].GetSRV(); }
    private:
        std::vector<GPUTexture> m_textures;
        std::unordered_map<std::string, size_t> m_textures_by_name;
    };
}
