#pragma once

#include <Dragon/Win32Utils.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>

#include <vector>

namespace D3D11Utils
{
    class SubresourceMapping
    {
    public:
        SubresourceMapping(ID3D11DeviceContext* context, ID3D11Resource* resource, UINT subresource_idx, D3D11_MAP map_type, UINT map_flags);
        ~SubresourceMapping();
        SubresourceMapping(const SubresourceMapping&) = delete;
        SubresourceMapping(SubresourceMapping&&) noexcept = delete;
        SubresourceMapping& operator=(const SubresourceMapping&) = delete;
        SubresourceMapping& operator=(SubresourceMapping&&) noexcept = delete;
    public:
        D3D11_MAPPED_SUBRESOURCE& GetSubresource() { return m_mapped_subresource; }
        const D3D11_MAPPED_SUBRESOURCE& GetSubresource() const { return m_mapped_subresource; }
    private:
        ID3D11DeviceContext* m_context;
        ID3D11Resource* m_resource;
        UINT m_subresource_idx;
        D3D11_MAPPED_SUBRESOURCE m_mapped_subresource;
    };

    wrl::ComPtr<ID3D11Device> CreateDevice();
    wrl::ComPtr<ID3D11DeviceContext> GetDeviceContext(ID3D11Device* device);
    void BreakOnErrors(ID3D11Device* device);
    wrl::ComPtr<IDXGISwapChain1> CreateSwapChain(ID3D11Device* device, HWND hwnd, DXGI_FORMAT back_buffer_format);
    wrl::ComPtr<ID3D11Texture2D> GetSwapChainBackBuffer(IDXGISwapChain1* swap_chain);
    std::vector<UINT> GetMSAASamplesForFormat(ID3D11Device* device, const std::vector<DXGI_FORMAT>& formats);
    std::vector<UINT> GetAnisotropyLevels();
    wrl::ComPtr<ID3D11RenderTargetView> CreateRTV(ID3D11Device* device, ID3D11Resource* resource);
    std::tuple<wrl::ComPtr<ID3D11VertexShader>, wrl::ComPtr<ID3DBlob>> LoadVertexShaderFromFile(ID3D11Device* device, const std::string& path);
    wrl::ComPtr<ID3D11PixelShader> LoadPixelShaderFromFile(ID3D11Device* device, const std::string& path);
    wrl::ComPtr<ID3D11VertexShader> LoadVertexShaderFromBytecode(ID3D11Device* device, const void* bytecode, SIZE_T bytecode_length);
    wrl::ComPtr<ID3D11PixelShader> LoadPixelShaderFromBytecode(ID3D11Device* device, const void* bytecode, SIZE_T bytecode_length);
    wrl::ComPtr<ID3D11InputLayout> CreateInputLayout(ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC* desc, UINT num_elements, ID3DBlob* vs_blob);
    wrl::ComPtr<ID3D11InputLayout> CreateInputLayout(ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC* desc, UINT num_elements, const void* vs_bytecode, SIZE_T vs_bytecode_length);
    wrl::ComPtr<ID3D11Buffer> CreateBuffer(ID3D11Device* device, D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subres_data);
    wrl::ComPtr<ID3D11Texture2D> CreateTexture2D(ID3D11Device* device, D3D11_TEXTURE2D_DESC* desc, D3D11_SUBRESOURCE_DATA* data = nullptr);
    wrl::ComPtr<ID3D11DepthStencilView> CreateDSV(ID3D11Device* device, ID3D11Resource* resource, D3D11_DEPTH_STENCIL_VIEW_DESC* desc = nullptr);
    wrl::ComPtr<ID3D11ShaderResourceView> CreateSRV(ID3D11Device* device, ID3D11Resource* resource, const D3D11_SHADER_RESOURCE_VIEW_DESC* desc = nullptr);
    wrl::ComPtr<ID3D11Texture2D> GetTexture2DFromRTV(ID3D11RenderTargetView* rtv);
}
