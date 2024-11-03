#include <Dragon/pch.h>
#include <Dragon/SwapChain.h>

namespace Dragon
{
    SwapChain::SwapChain(ID3D11Device* device, ID3D11DeviceContext* context, HWND hwnd, unsigned msaa_sample_count)
        : m_device{ device }
        , m_context{ context }
        , m_swap_chain{ D3D11Utils::CreateSwapChain(m_device, hwnd, DRAGON_SWAP_CHAIN_BACK_BUFFER_FORMAT) }
        , m_rtv{}
        , m_dsv{}
    {
        CreateRTVAndDSV(msaa_sample_count);
    }

    void SwapChain::SetMSAASampleCount(unsigned msaa_sample_count)
    {
        Dragon_Check(1 <= msaa_sample_count && msaa_sample_count <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT);

        m_context->ClearState();
        m_rtv.Reset();
        m_dsv.Reset();

        CreateRTVAndDSV(msaa_sample_count);
    }

    void SwapChain::Resize()
    {
        // NOTE: get MSAA sample count from rtv
        unsigned msaa_sample_count{};
        {
            auto color_buffer{ D3D11Utils::GetTexture2DFromRTV(m_rtv.Get()) };
            D3D11_TEXTURE2D_DESC color_buffer_desc{};
            color_buffer->GetDesc(&color_buffer_desc);
            msaa_sample_count = color_buffer_desc.SampleDesc.Count;
        }

        m_context->ClearState();
        m_rtv.Reset();
        m_dsv.Reset();

        Dragon_CheckHR(m_swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));
        CreateRTVAndDSV(msaa_sample_count);
    }

    void SwapChain::Present(bool vsync)
    {
        auto back_buffer{ D3D11Utils::GetSwapChainBackBuffer(m_swap_chain.Get()) };
        auto color_buffer{ D3D11Utils::GetTexture2DFromRTV(m_rtv.Get()) };
        D3D11_TEXTURE2D_DESC color_buffer_desc{};
        color_buffer->GetDesc(&color_buffer_desc);
        unsigned msaa_sample_count{ color_buffer_desc.SampleDesc.Count };
        if (msaa_sample_count == 1)
        {
            m_context->CopyResource(back_buffer.Get(), color_buffer.Get());
        }
        else
        {
            m_context->ResolveSubresource(back_buffer.Get(), 0, color_buffer.Get(), 0, DRAGON_SWAP_CHAIN_BACK_BUFFER_FORMAT);
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

    void SwapChain::CreateRTVAndDSV(unsigned msaa_sample_count)
    {
        Dragon_Check(1 <= msaa_sample_count && msaa_sample_count <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT);

        auto back_buffer{ D3D11Utils::GetSwapChainBackBuffer(m_swap_chain.Get()) };
        D3D11_TEXTURE2D_DESC buffer_desc{};
        back_buffer->GetDesc(&buffer_desc);

        buffer_desc.BindFlags = D3D11_BIND_RENDER_TARGET;
        buffer_desc.Format = DRAGON_SWAP_CHAIN_BACK_BUFFER_FORMAT;
        buffer_desc.SampleDesc.Count = msaa_sample_count;
        buffer_desc.SampleDesc.Quality = buffer_desc.SampleDesc.Count == 1 ? 0 : D3D11_STANDARD_MULTISAMPLE_PATTERN;
        auto color_buffer{ D3D11Utils::CreateTexture2D(m_device, &buffer_desc) };
        m_rtv = D3D11Utils::CreateRTV(m_device, color_buffer.Get());

        buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        buffer_desc.Format = DRAGON_SWAP_CHAIN_DEPTH_STENCIL_FORMAT;
        buffer_desc.SampleDesc.Count = msaa_sample_count;
        buffer_desc.SampleDesc.Quality = buffer_desc.SampleDesc.Count == 1 ? 0 : D3D11_STANDARD_MULTISAMPLE_PATTERN;
        auto depth_buffer{ D3D11Utils::CreateTexture2D(m_device, &buffer_desc) };
        m_dsv = D3D11Utils::CreateDSV(m_device, depth_buffer.Get());
    }
        }