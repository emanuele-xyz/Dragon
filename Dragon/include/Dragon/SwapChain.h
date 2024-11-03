#pragma once

#include <Dragon/D3D11Utils.h>

#include <vector>

#define DRAGON_SWAP_CHAIN_BACK_BUFFER_FORMAT DXGI_FORMAT_R8G8B8A8_UNORM
#define DRAGON_SWAP_CHAIN_DEPTH_STENCIL_FORMAT DXGI_FORMAT_D24_UNORM_S8_UINT

namespace Dragon
{
    class SwapChain
    {
    public:
        SwapChain(ID3D11Device* device, ID3D11DeviceContext* context, HWND hwnd, unsigned msaa_sample_count);
        ~SwapChain() = default;
        SwapChain(const SwapChain&) = delete;
        SwapChain(SwapChain&&) noexcept = delete;
        SwapChain& operator=(const SwapChain&) = delete;
        SwapChain& operator=(SwapChain&&) noexcept = delete;
    public:
        ID3D11RenderTargetView* GetRTV() { return m_rtv.Get(); }
        ID3D11DepthStencilView* GetDSV() { return m_dsv.Get(); }
    public:
        void SetMSAASampleCount(unsigned msaa_sample_count);
        void Resize();
        void Present(bool vsync);
    private:
        void CreateRTVAndDSV(unsigned msaa_sample_count);
    private:
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_context;
        wrl::ComPtr<IDXGISwapChain1> m_swap_chain;
        wrl::ComPtr<ID3D11RenderTargetView> m_rtv;
        wrl::ComPtr<ID3D11DepthStencilView> m_dsv;
    };
}
