#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include <Dragon/D3D11Utils.h>

namespace Dragon
{
    class PixelShaderManager
    {
    public:
        PixelShaderManager() = default;
        ~PixelShaderManager() = default;
        PixelShaderManager(const PixelShaderManager&) = delete;
        PixelShaderManager(PixelShaderManager&&) noexcept = delete;
        PixelShaderManager& operator=(const PixelShaderManager&) = delete;
        PixelShaderManager& operator=(PixelShaderManager&&) noexcept = delete;
    public:
        size_t RegisterShader(wrl::ComPtr<ID3D11PixelShader> shader, const std::string& name);
    private:
        std::vector<wrl::ComPtr<ID3D11PixelShader>> m_shaders;
        std::unordered_map<std::string, size_t> m_shaders_by_name;
    };
}
