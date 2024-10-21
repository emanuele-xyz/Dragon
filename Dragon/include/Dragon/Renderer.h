#pragma once

#include <Dragon/D3D11Utils.h>
#include <Dragon/GfxResources.h>

#include <Dragon/GPUMesh.h> // TODO: to be removed?
#include <Dragon/GPUTexture.h> // TODO: to be removed?

namespace Dragon
{
    class Renderer
    {
    public:
        Renderer(ID3D11Device* device, ID3D11DeviceContext* context, GfxResources* gfx_resources); // TODO: pass resources by refeerence!
        ~Renderer() = default;
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = delete;
    public:
        void Render(float view_w, float view_h, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, GPUMesh* dragon_mesh, GPUTexture* dragon_texture);
    private:
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_context;
        GfxResources* m_gfx_resources;
        wrl::ComPtr<ID3D11Buffer> m_cb_camera;
        wrl::ComPtr<ID3D11Buffer> m_cb_object;
        ID3D11Buffer* m_constant_buffers[2];
    };
}
