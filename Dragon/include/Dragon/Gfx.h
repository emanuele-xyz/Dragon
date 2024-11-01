#pragma once

#include <Dragon/D3D11Utils.h>

#include <vector>

namespace Dragon
{
    class Gfx
    {
    public:
        Gfx(HWND hwnd);
        ~Gfx() = default;
        Gfx(const Gfx&) = delete;
        Gfx(Gfx&&) noexcept = delete;
        Gfx& operator=(const Gfx&) = delete;
        Gfx& operator=(Gfx&&) noexcept = delete;
    public:
        ID3D11Device* GetDevice() { return m_device.Get(); }
        ID3D11DeviceContext* GetContext() { return m_context.Get(); }
        ID3D11RenderTargetView* GetBackBufferRTV() { return m_rtv.Get(); }
        ID3D11DepthStencilView* GetBackBufferDSV() { return m_dsv.Get(); }
        size_t GetMSAAIndex() const { return m_msaa_index; }
        void SetMSAAIndex(size_t idx);
        std::span<const UINT> GetSupportedMSAASamples() const { return m_supported_msaa_samples; }
    public:
        void Resize();
        void Present(bool vsync);
    private:
        void CreateRTVAndDSV();
    private:
        wrl::ComPtr<ID3D11Device> m_device;
        wrl::ComPtr<ID3D11DeviceContext> m_context;
        wrl::ComPtr<IDXGISwapChain1> m_swap_chain;
        std::vector<UINT> m_supported_msaa_samples;
        size_t m_msaa_index;
        wrl::ComPtr<ID3D11RenderTargetView> m_rtv;
        wrl::ComPtr<ID3D11DepthStencilView> m_dsv;
    };
}
