#include <Dragon/pch.h>
#include <Dragon/Gfx.h>

namespace Dragon
{
    static wrl::ComPtr<ID3D11DepthStencilView> CreateBackBufferDSV(ID3D11Device* device, ID3D11Texture2D* back_buffer)
    {
        D3D11_TEXTURE2D_DESC desc{};
        back_buffer->GetDesc(&desc);
        desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        auto depth_buffer{ D3D11Utils::CreateTexture2D(device, &desc) };
        return D3D11Utils::CreateDSV(device, depth_buffer.Get());
    }

    Gfx::Gfx(HWND hwnd)
    {
        m_device = D3D11Utils::CreateDevice();
        m_context = D3D11Utils::GetDeviceContext(m_device.Get());
        #if defined(_DEBUG)
        D3D11Utils::BreakOnErrors(m_device.Get());
        #endif
        m_swap_chain = D3D11Utils::CreateSwapChain(m_device.Get(), hwnd);
        auto swap_chain_back_buffer{ D3D11Utils::GetSwapChainBackBuffer(m_swap_chain.Get()) };
        m_back_buffer_rtv = D3D11Utils::CreateRTV(m_device.Get(), swap_chain_back_buffer.Get());

        D3D11_TEXTURE2D_DESC depth_buffer_desc{};
        swap_chain_back_buffer->GetDesc(&depth_buffer_desc);
        depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        auto depth_buffer{ D3D11Utils::CreateTexture2D(m_device.Get(), &depth_buffer_desc) };
        m_back_buffer_dsv = D3D11Utils::CreateDSV(m_device.Get(), depth_buffer.Get());
    }

    void Gfx::Resize()
    {
        m_context->ClearState();

        m_back_buffer_rtv.Reset();
        m_back_buffer_dsv.Reset();

        Dragon_CheckHR(m_swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

        auto swap_chain_back_buffer{ D3D11Utils::GetSwapChainBackBuffer(m_swap_chain.Get()) };
        m_back_buffer_rtv = D3D11Utils::CreateRTV(m_device.Get(), swap_chain_back_buffer.Get());
        m_back_buffer_dsv = CreateBackBufferDSV(m_device.Get(), swap_chain_back_buffer.Get());
    }

    void Gfx::Present(bool vsync)
    {
        Dragon_CheckHR(m_swap_chain->Present(vsync ? 1 : 0, 0));

        // TODO: if present ever crashes, then fix it
        #if 0
        HRESULT hr = IDXGISwapChain_Present(swap_chain, 1, 0);
        if (hr == DXGI_ERROR_DEVICE_RESET || hr == DXGI_ERROR_DEVICE_REMOVED) {
            /* to recover from this, you'll need to recreate device and all the resources */
            MessageBoxW(NULL, L"D3D11 device is lost or removed!", L"Error", 0);
            break;
        }
        else if (hr == DXGI_STATUS_OCCLUDED) {
            /* window is not visible, so vsync won't work. Let's sleep a bit to reduce CPU usage */
            Sleep(10);
        }
        #endif
    }
}
