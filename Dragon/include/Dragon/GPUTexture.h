#pragma once

#include <Dragon/D3D11Utils.h>
#include <Dragon/CPUTexture.h>

namespace Dragon
{
    class GPUTexture
    {
    public:
        // TODO: specify flag for generating mipmaps (do it after doing CPUTexture TODOs)
        static GPUTexture FromCPUTexture(ID3D11Device* device, const CPUTexture& cpu_texture);
    public:
        GPUTexture() : m_texture{}, m_texture_srv{} {}
        ~GPUTexture() = default;
        GPUTexture(const GPUTexture&) = delete;
        GPUTexture(GPUTexture&&) noexcept = default;
        GPUTexture& operator=(const GPUTexture&) = delete;
        GPUTexture& operator=(GPUTexture&&) noexcept = default;
    public:
        ID3D11ShaderResourceView** GetAddressOfSRV() { return m_texture_srv.GetAddressOf(); }
    private:
        wrl::ComPtr<ID3D11Texture2D> m_texture;
        wrl::ComPtr<ID3D11ShaderResourceView> m_texture_srv;
    };
}
