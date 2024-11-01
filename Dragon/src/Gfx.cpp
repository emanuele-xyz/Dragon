#include <Dragon/pch.h>
#include <Dragon/Gfx.h>

#define DRAGON_BACK_BUFFER_FORMAT DXGI_FORMAT_R8G8B8A8_UNORM
#define DRAGON_DEPTH_STENCIL_FORMAT DXGI_FORMAT_D24_UNORM_S8_UINT

namespace Dragon
{
    Gfx::Gfx(HWND hwnd)
    {
        m_device = D3D11Utils::CreateDevice();
        m_context = D3D11Utils::GetDeviceContext(m_device.Get());
        #if defined(_DEBUG)
        D3D11Utils::BreakOnErrors(m_device.Get());
        #endif
        m_swap_chain = D3D11Utils::CreateSwapChain(m_device.Get(), hwnd, DRAGON_BACK_BUFFER_FORMAT);
        m_supported_msaa_samples = D3D11Utils::GetMSAASamplesForFormat(m_device.Get(), { DRAGON_BACK_BUFFER_FORMAT, DRAGON_DEPTH_STENCIL_FORMAT });
        Dragon_Check(m_supported_msaa_samples.size() > 0);
        m_msaa_index = 0; /*m_supported_msaa_samples.size() - 1;*/
        CreateRTVAndDSV();
    }

    void Gfx::Resize()
    {
        m_context->ClearState();
        m_rtv.Reset();
        m_dsv.Reset();

        Dragon_CheckHR(m_swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));
        CreateRTVAndDSV();
    }

    void Gfx::Present(bool vsync)
    {
        auto back_buffer{ D3D11Utils::GetSwapChainBackBuffer(m_swap_chain.Get()) };
        auto color_buffer{ D3D11Utils::GetTexture2DFromRTV(m_rtv.Get()) };
        if (m_supported_msaa_samples[m_msaa_index] == 1)
        {
            m_context->CopyResource(back_buffer.Get(), color_buffer.Get());
        }
        else
        {
            m_context->ResolveSubresource(back_buffer.Get(), 0, color_buffer.Get(), 0, DRAGON_BACK_BUFFER_FORMAT);
        }

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

    void Gfx::CreateRTVAndDSV()
    {
        auto back_buffer{ D3D11Utils::GetSwapChainBackBuffer(m_swap_chain.Get()) };
        D3D11_TEXTURE2D_DESC buffer_desc{};
        back_buffer->GetDesc(&buffer_desc);

        buffer_desc.BindFlags = D3D11_BIND_RENDER_TARGET;
        buffer_desc.Format = DRAGON_BACK_BUFFER_FORMAT;
        buffer_desc.SampleDesc.Count = m_supported_msaa_samples[m_msaa_index];
        buffer_desc.SampleDesc.Quality = buffer_desc.SampleDesc.Count == 1 ? 0 : D3D11_STANDARD_MULTISAMPLE_PATTERN;
        auto color_buffer{ D3D11Utils::CreateTexture2D(m_device.Get(), &buffer_desc) };
        m_rtv = D3D11Utils::CreateRTV(m_device.Get(), color_buffer.Get());

        buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        buffer_desc.Format = DRAGON_DEPTH_STENCIL_FORMAT;
        buffer_desc.SampleDesc.Count = m_supported_msaa_samples[m_msaa_index];
        buffer_desc.SampleDesc.Quality = buffer_desc.SampleDesc.Count == 1 ? 0 : D3D11_STANDARD_MULTISAMPLE_PATTERN;
        auto depth_buffer{ D3D11Utils::CreateTexture2D(m_device.Get(), &buffer_desc) };
        m_dsv = D3D11Utils::CreateDSV(m_device.Get(), depth_buffer.Get());
    }
}
