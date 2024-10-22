#include <Dragon/pch.h>
#include <Dragon/TextureManager.h>

namespace Dragon
{
    size_t TextureManager::RegisterTexture(GPUTexture&& texture, const std::string& name)
    {
        Dragon_Check(!m_textures_by_name.contains(name));
        size_t idx{ m_textures.size() };
        m_textures.emplace_back(std::move(texture));
        m_textures_by_name.insert({ name, idx });
        return idx;
    }
}