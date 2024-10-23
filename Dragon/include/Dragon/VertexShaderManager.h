#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include <Dragon/D3D11Utils.h>

namespace Dragon
{
    class VertexShaderManager
    {
    public:
        VertexShaderManager() = default;
        ~VertexShaderManager() = default;
        VertexShaderManager(const VertexShaderManager&) = delete;
        VertexShaderManager(VertexShaderManager&&) noexcept = delete;
        VertexShaderManager& operator=(const VertexShaderManager&) = delete;
        VertexShaderManager& operator=(VertexShaderManager&&) noexcept = delete;
    public:
        size_t RegisterShader(wrl::ComPtr<ID3D11VertexShader> shader, const std::string& name);
    private:
        std::vector<wrl::ComPtr<ID3D11VertexShader>> m_shaders;
        std::unordered_map<std::string, size_t> m_shaders_by_name;
    };
}
