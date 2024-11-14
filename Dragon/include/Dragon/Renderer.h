#pragma once

#include <Dragon/D3D11Utils.h>
#include <Dragon/MathUtils.h>
#include <Dragon/MeshRef.h>
#include <Dragon/TextureRef.h>

#include <vector>
#include <memory>

namespace Dragon
{
    struct RenderCfg
    {
        Vector3 position{};
        Quaternion rotation{ Quaternion::Identity };
        Vector3 scaling{ Vector3::One };
        MeshRef mesh{};
        TextureRef texture{};
        bool is_lit{ true };
        Vector3 color{ Vector3::One };
        float blend_factor{ 0.0f }; // 0 for albedo texture, 1 for full color
    };

    class Renderer
    {
    public:
        Renderer(ID3D11Device* device, ID3D11DeviceContext* context);
        ~Renderer() = default;
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = delete;
    public:
        void NewFrame(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, float w, float h);
        void SetCamera(const Matrix& view, const Matrix& projection);
        void SetLighting(Vector3 ambient_color, Vector3 light_direction, Vector3 light_color);
        void Render(RenderCfg& cfg);
        void RenderAABB(Vector3 p_min, Vector3 p_max, Vector3 color);
    public:
        void SetAnisotropy(unsigned anisotropy);
        size_t GetAnisotropyIndex() { return 0; }
    private:
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_context;
        wrl::ComPtr<ID3D11VertexShader> m_vs_default;
        wrl::ComPtr<ID3D11PixelShader> m_ps_default;
        wrl::ComPtr<ID3D11InputLayout> m_input_layout;
        wrl::ComPtr<ID3D11RasterizerState> m_rasterizer_state_default;
        wrl::ComPtr<ID3D11DepthStencilState> m_depth_stencil_state_default;
        wrl::ComPtr<ID3D11SamplerState> m_sampler_state_default;
        wrl::ComPtr<ID3D11Buffer> m_cb_camera;
        wrl::ComPtr<ID3D11Buffer> m_cb_object;
        wrl::ComPtr<ID3D11Buffer> m_cb_lighting;
        std::vector<ID3D11Buffer*> m_constant_buffers;
        std::unique_ptr<Mesh> m_aabb_mesh; // TODO: should we make Mesh movable and/or copiable?
    };
}
