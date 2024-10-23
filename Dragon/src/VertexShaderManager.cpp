#include <Dragon/pch.h>
#include <Dragon/VertexShaderManager.h>

namespace Dragon
{
    size_t VertexShaderManager::RegisterShader(wrl::ComPtr<ID3D11VertexShader> shader, const std::string& name)
    {
        Dragon_Check(!m_shaders_by_name.contains(name));
        size_t idx{ m_shaders.size() };
        m_shaders.emplace_back(shader);
        m_shaders_by_name.insert({ name, idx });
        return idx;
    }
}