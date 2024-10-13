#pragma once

#include <Dragon/D3D11Utils.h>

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
        ID3D11RenderTargetView* GetBackBufferRTV() { return m_back_buffer_rtv.Get(); }
        ID3D11DepthStencilView* GetBackBufferDSV() { return m_back_buffer_dsv.Get(); }
    public:
        void Resize();
        void Present(bool vsync);
    private:
        wrl::ComPtr<ID3D11Device> m_device;
        wrl::ComPtr<ID3D11DeviceContext> m_context;
        wrl::ComPtr<IDXGISwapChain1> m_swap_chain;
        wrl::ComPtr<ID3D11RenderTargetView> m_back_buffer_rtv;
        wrl::ComPtr<ID3D11DepthStencilView> m_back_buffer_dsv;
    };
}
