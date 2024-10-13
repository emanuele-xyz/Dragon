#include <Dragon/pch.h>
#include <Dragon/D3D11Utils.h>

#pragma comment (lib, "dxguid")
#pragma comment (lib, "dxgi")
#pragma comment (lib, "d3d11")
#pragma comment (lib, "d3dcompiler")

namespace D3D11Utils
{
    SubresourceMapping::SubresourceMapping(ID3D11DeviceContext* context, ID3D11Resource* resource, UINT subresource_idx, D3D11_MAP map_type, UINT map_flags)
        : m_context{ context }
        , m_resource{ resource }
        , m_subresource_idx{ subresource_idx }
        , m_mapped_subresource{}
    {
        Dragon_CheckHR(context->Map(resource, subresource_idx, map_type, map_flags, &m_mapped_subresource));
    }

    SubresourceMapping::~SubresourceMapping()
    {
        m_context->Unmap(m_resource, m_subresource_idx);
    }

    wrl::ComPtr<ID3D11Device> CreateDevice()
    {
        wrl::ComPtr<ID3D11Device> device{};
        UINT flags{};
        #if defined(_DEBUG)
        flags |= D3D11_CREATE_DEVICE_DEBUG;
        #endif
        D3D_FEATURE_LEVEL levels[]{ D3D_FEATURE_LEVEL_11_0 };
        Dragon_CheckHR(D3D11CreateDevice(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levels, Dragon_CountOf(levels), D3D11_SDK_VERSION, device.ReleaseAndGetAddressOf(), nullptr, nullptr)
        );
        return device;
    }

    wrl::ComPtr<ID3D11DeviceContext> GetDeviceContext(ID3D11Device* device)
    {
        wrl::ComPtr<ID3D11DeviceContext> ctx{};
        device->GetImmediateContext(ctx.ReleaseAndGetAddressOf());
        return ctx;
    }

    void BreakOnErrors(ID3D11Device* device)
    {
        {
            wrl::ComPtr<ID3D11InfoQueue> queue{};
            Dragon_CheckHR(device->QueryInterface(queue.ReleaseAndGetAddressOf()));
            Dragon_CheckHR(queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE));
            Dragon_CheckHR(queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE));
        }
        {
            wrl::ComPtr<IDXGIInfoQueue> queue{};
            Dragon_CheckHR(DXGIGetDebugInterface1(0, __uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(queue.ReleaseAndGetAddressOf())));
            Dragon_CheckHR(queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, TRUE));
            Dragon_CheckHR(queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, TRUE));
        }
    }

    wrl::ComPtr<IDXGISwapChain1> CreateSwapChain(ID3D11Device* device, HWND hwnd)
    {
        wrl::ComPtr<IDXGIDevice> dxgi_device{};
        Dragon_CheckHR(device->QueryInterface(dxgi_device.ReleaseAndGetAddressOf()));
        wrl::ComPtr<IDXGIAdapter> dxgi_adapter{};
        Dragon_CheckHR(dxgi_device->GetAdapter(dxgi_adapter.ReleaseAndGetAddressOf()));
        wrl::ComPtr<IDXGIFactory2> dxgi_factory{};
        Dragon_CheckHR(dxgi_adapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(dxgi_factory.ReleaseAndGetAddressOf())));

        DXGI_SWAP_CHAIN_DESC1 desc{};
        desc.Width = 0; // NOTE: use window width
        desc.Height = 0; // NOTE: use window height
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Stereo = FALSE;
        desc.SampleDesc = { .Count = 1, .Quality = 0 };
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = 2;
        desc.Scaling = DXGI_SCALING_NONE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        desc.Flags = 0;

        wrl::ComPtr<IDXGISwapChain1> swap_chain{};
        Dragon_CheckHR(dxgi_factory->CreateSwapChainForHwnd(device, hwnd, &desc, nullptr, nullptr, swap_chain.ReleaseAndGetAddressOf()));

        // NOTE: disable Alt+Enter changing monitor resolution to match window size
        Dragon_CheckHR(dxgi_factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

        return swap_chain;
    }

    wrl::ComPtr<ID3D11Texture2D> GetSwapChainBackBuffer(IDXGISwapChain1* swap_chain)
    {
        wrl::ComPtr<ID3D11Texture2D> back_buffer{};
        Dragon_CheckHR(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(back_buffer.ReleaseAndGetAddressOf())));
        return back_buffer;
    }

    wrl::ComPtr<ID3D11RenderTargetView> CreateRTV(ID3D11Device* device, ID3D11Resource* resource)
    {
        wrl::ComPtr<ID3D11RenderTargetView> rtv{};
        Dragon_CheckHR(device->CreateRenderTargetView(resource, nullptr, rtv.ReleaseAndGetAddressOf()));
        return rtv;
    }

    std::tuple<wrl::ComPtr<ID3D11VertexShader>, wrl::ComPtr<ID3DBlob>> LoadVertexShaderFromFile(ID3D11Device* device, const std::string& path)
    {
        std::wstring wpath{ Dragon::GetWStrFromStr(path) };
        wrl::ComPtr<ID3DBlob> blob{};
        wrl::ComPtr<ID3D11VertexShader> shader{};
        Dragon_CheckHR(D3DReadFileToBlob(wpath.c_str(), blob.ReleaseAndGetAddressOf()));
        Dragon_CheckHR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, shader.ReleaseAndGetAddressOf()));
        return { std::move(shader), std::move(blob) };
    }

    wrl::ComPtr<ID3D11PixelShader> LoadPixelShaderFromFile(ID3D11Device* device, const std::string& path)
    {
        std::wstring wpath{ Dragon::GetWStrFromStr(path) };
        wrl::ComPtr<ID3DBlob> blob{};
        wrl::ComPtr<ID3D11PixelShader> shader{};
        Dragon_CheckHR(D3DReadFileToBlob(wpath.c_str(), blob.ReleaseAndGetAddressOf()));
        Dragon_CheckHR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, shader.ReleaseAndGetAddressOf()));
        return shader;
    }

    wrl::ComPtr<ID3D11InputLayout> CreateInputLayout(ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC* desc, UINT num_elements, ID3DBlob* vs_blob)
    {
        wrl::ComPtr<ID3D11InputLayout> input_layout{};
        Dragon_CheckHR(device->CreateInputLayout(desc, num_elements, vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), input_layout.ReleaseAndGetAddressOf()));
        return input_layout;
    }

    wrl::ComPtr<ID3D11Buffer> CreateBuffer(ID3D11Device* device, D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subres_data)
    {
        wrl::ComPtr<ID3D11Buffer> buffer{};
        Dragon_CheckHR(device->CreateBuffer(desc, subres_data, buffer.ReleaseAndGetAddressOf()));
        return buffer;
    }

    wrl::ComPtr<ID3D11Texture2D> CreateTexture2D(ID3D11Device* device, D3D11_TEXTURE2D_DESC* desc, D3D11_SUBRESOURCE_DATA* data)
    {
        wrl::ComPtr<ID3D11Texture2D> texture{};
        Dragon_CheckHR(device->CreateTexture2D(desc, data, texture.ReleaseAndGetAddressOf()));
        return texture;
    }

    wrl::ComPtr<ID3D11DepthStencilView> CreateDSV(ID3D11Device* device, ID3D11Resource* resource, D3D11_DEPTH_STENCIL_VIEW_DESC* desc)
    {
        wrl::ComPtr<ID3D11DepthStencilView> dsv{};
        Dragon_CheckHR(device->CreateDepthStencilView(resource, desc, dsv.ReleaseAndGetAddressOf()));
        return dsv;
    }

    wrl::ComPtr<ID3D11ShaderResourceView> CreateSRV(ID3D11Device* device, ID3D11Resource* resource, const D3D11_SHADER_RESOURCE_VIEW_DESC* desc)
    {
        wrl::ComPtr<ID3D11ShaderResourceView> srv{};
        Dragon_CheckHR(device->CreateShaderResourceView(resource, desc, srv.ReleaseAndGetAddressOf()));
        return srv;
    }
}
