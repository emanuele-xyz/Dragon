#include <Dragon/pch.h>
#include <Dragon/PixelShaderManager.h>

namespace Dragon
{
    size_t PixelShaderManager::RegisterShader(wrl::ComPtr<ID3D11PixelShader> shader, const std::string& name)
    {
        Dragon_Check(!m_shaders_by_name.contains(name));
        size_t idx{ m_shaders.size() };
        m_shaders.emplace_back(shader);
        m_shaders_by_name.insert({ name, idx });
        return idx;
    }
}