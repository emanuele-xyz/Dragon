#pragma once

#include <Dragon/D3D11Utils.h>

namespace Dragon
{
    class GfxResources
    {
    public:
        GfxResources(ID3D11Device* device);
        ~GfxResources() = default;
        GfxResources(const GfxResources&) = delete;
        GfxResources(GfxResources&&) noexcept = delete;
        GfxResources& operator=(const GfxResources&) = delete;
        GfxResources& operator=(GfxResources&&) noexcept = delete;
    public:
        ID3D11VertexShader* GetVSDefault() { return m_vs_default.Get(); }
        ID3D11PixelShader* GetPSUnlit() { return m_ps_unlit.Get(); }
        ID3D11InputLayout* GetInputLayout() { return m_input_layout.Get(); };
        ID3D11RasterizerState* GetRSDefault() { return m_rasterizer_state_default.Get(); };
        ID3D11DepthStencilState* GetDSSDefault() { return m_depth_stencil_state_default.Get(); };
        ID3D11SamplerState* GetSSDefault() { return m_sampler_state_default.Get(); };
        ID3D11Buffer* GetCBCamera() { return m_cb_camera.Get(); };
        ID3D11Buffer* GetCBObject() { return m_cb_object.Get(); };
        ID3D11Buffer** GetConstantBuffers() { return m_constant_buffers; }
        UINT GetConstantBuffersCount() { return Dragon_CountOf(m_constant_buffers); }
    private:
        ID3D11Device* m_device;
        wrl::ComPtr<ID3D11VertexShader> m_vs_default;
        wrl::ComPtr<ID3D11PixelShader> m_ps_unlit;
        wrl::ComPtr<ID3D11InputLayout> m_input_layout;
        wrl::ComPtr<ID3D11RasterizerState> m_rasterizer_state_default;
        wrl::ComPtr<ID3D11DepthStencilState> m_depth_stencil_state_default;
        wrl::ComPtr<ID3D11SamplerState> m_sampler_state_default;
        wrl::ComPtr<ID3D11Buffer> m_cb_camera;
        wrl::ComPtr<ID3D11Buffer> m_cb_object;
        ID3D11Buffer* m_constant_buffers[2];
    };
}
