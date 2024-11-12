#include <Dragon/pch.h>
#include <Dragon/Renderer.h>

#define float3 Vector3
#define float4 Vector4
#define matrix Matrix
#include <Dragon/hlsl/CBCamera.h>
#include <Dragon/hlsl/CBObject.h>
#include <Dragon/hlsl/CBLighting.h>
#undef float3
#undef float4
#undef matrix

namespace Dragon
{
    Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* context)
        : m_device{ device }
        , m_context{ context }
    {
        // NOTE: shaders and input layout
        {
            #if defined(_DEBUG)
            #include <Dragon/hlsl/DefaultVSDebug.h> // for vs_default_bytecode
            #include <Dragon/hlsl/DefaultPSDebug.h> // for ps_default_bytecode
            #else
            #include <Dragon/hlsl/DefaultVSRelease.h> // for vs_default_bytecode
            #include <Dragon/hlsl/DefaultPSRelease.h> // for ps_default_bytecode
            #endif

            m_vs_default = D3D11Utils::LoadVertexShaderFromBytecode(m_device, vs_default_bytecode, Dragon_CountOf(vs_default_bytecode));
            m_ps_default = D3D11Utils::LoadPixelShaderFromBytecode(m_device, ps_default_bytecode, Dragon_CountOf(ps_default_bytecode));

            D3D11_INPUT_ELEMENT_DESC desc[]
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            m_input_layout = D3D11Utils::CreateInputLayout(m_device, desc, Dragon_CountOf(desc), vs_default_bytecode, Dragon_CountOf(vs_default_bytecode));
        }

        // NOTE: default rasterizer state
        {
            // NOTE: https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_rasterizer_desc
            D3D11_RASTERIZER_DESC desc{};
            desc.FillMode = D3D11_FILL_SOLID;
            desc.CullMode = D3D11_CULL_BACK;
            desc.FrontCounterClockwise = TRUE;
            desc.DepthBias = 0;
            desc.DepthBiasClamp = 0.0f;
            desc.SlopeScaledDepthBias = 0.0f;
            desc.DepthClipEnable = TRUE;
            desc.ScissorEnable = FALSE;
            // NOTE: we recommend that you always set MultisampleEnable to TRUE whenever you render on MSAA render targets
            desc.MultisampleEnable = FALSE;
            // NOTE: it only applies if doing line drawing and MultisampleEnable is FALSE
            desc.AntialiasedLineEnable = FALSE;

            Dragon_CheckHR(m_device->CreateRasterizerState(&desc, m_rasterizer_state_default.ReleaseAndGetAddressOf()));
        }

        // NOTE: default depth stencil state
        {
            D3D11_DEPTH_STENCIL_DESC desc{};
            desc.DepthEnable = TRUE;
            desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            desc.DepthFunc = D3D11_COMPARISON_LESS;
            //desc.StencilEnable = ;
            //desc.StencilReadMask = ;
            //desc.StencilWriteMask = ;
            //desc.FrontFace = ;
            //desc.BackFace = ;

            Dragon_CheckHR(m_device->CreateDepthStencilState(&desc, m_depth_stencil_state_default.ReleaseAndGetAddressOf()));
        }

        // NOTE: create sampler state (default to no anisotropic filtering)
        {
            SetAnisotropy(0);
        }

        // NOTE: camera constants
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = sizeof(CBCamera);
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;
            Dragon_CheckHR(m_device->CreateBuffer(&desc, nullptr, m_cb_camera.ReleaseAndGetAddressOf()));
            m_constant_buffers.emplace_back(m_cb_camera.Get());
        }

        // NOTE: object constants
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = sizeof(CBObject);
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;
            Dragon_CheckHR(m_device->CreateBuffer(&desc, nullptr, m_cb_object.ReleaseAndGetAddressOf()));
            m_constant_buffers.emplace_back(m_cb_object.Get());
        }

        // NOTE: ambient and directional light constants
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = sizeof(CBLighting);
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;
            Dragon_CheckHR(m_device->CreateBuffer(&desc, nullptr, m_cb_lighting.ReleaseAndGetAddressOf()));
            m_constant_buffers.emplace_back(m_cb_lighting.Get());
        }
    }

    void Renderer::NewFrame(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, float w, float h)
    {
        float clear_color[4]{ 1.0f, 0.0f, 1.0f, 1.0f }; // TODO: change clear color
        m_context->ClearRenderTargetView(rtv, clear_color);
        m_context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

        D3D11_VIEWPORT viewport{};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = w;
        viewport.Height = h;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        m_context->RSSetViewports(1, &viewport);
        m_context->RSSetState(m_rasterizer_state_default.Get());
        m_context->OMSetRenderTargets(1, &rtv, dsv);
        m_context->OMSetDepthStencilState(m_depth_stencil_state_default.Get(), 0);
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_context->IASetInputLayout(m_input_layout.Get());
        m_context->VSSetShader(m_vs_default.Get(), nullptr, 0);
        m_context->VSSetConstantBuffers(0, static_cast<UINT>(m_constant_buffers.size()), m_constant_buffers.data());
        m_context->PSSetShader(m_ps_default.Get(), nullptr, 0);
        m_context->PSSetConstantBuffers(0, static_cast<UINT>(m_constant_buffers.size()), m_constant_buffers.data());
        m_context->PSSetSamplers(0, 1, m_sampler_state_default.GetAddressOf());
    }

    void Renderer::SetCamera(const Matrix& view, const Matrix& projection)
    {
        D3D11Utils::SubresourceMapping subres_mapping{ m_context, m_cb_camera.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
        auto constants{ static_cast<CBCamera*>(subres_mapping.GetSubresource().pData) };
        constants->view = view;
        constants->projection = projection;
    }

    void Renderer::SetLighting(Vector3 ambient_color, Vector3 light_direction, Vector3 light_color)
    {
        D3D11Utils::SubresourceMapping subres_mapping{ m_context, m_cb_lighting.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
        auto constants{ static_cast<CBLighting*>(subres_mapping.GetSubresource().pData) };
        constants->ambient_color = { ambient_color.x, ambient_color.y, ambient_color.z, 1.0f };
        constants->light_color = { light_color.x, light_color.y, light_color.z, 1.0f };
        constants->light_direction = { light_direction.x, light_direction.y, light_direction.z, 0.0f };
    }

    void Renderer::Render(RenderCfg& cfg)
    {
        // NOTE: update object constants
        {
            D3D11Utils::SubresourceMapping subres_mapping{ m_context, m_cb_object.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
            auto constants{ static_cast<CBObject*>(subres_mapping.GetSubresource().pData) };
            Matrix translate{ Matrix::CreateTranslation(cfg.position) };
            Matrix rotate{ Matrix::CreateFromQuaternion(cfg.rotation) };
            Matrix scale{ Matrix::CreateScale(cfg.scaling) };
            Matrix model{ scale * rotate * translate };
            Matrix normal{ scale * rotate };
            normal.Invert();
            normal.Transpose();
            constants->model = model;
            constants->normal = normal;
            constants->color = cfg.color;
            constants->blend_factor = cfg.blend_factor;
            constants->is_lit = static_cast<int>(cfg.is_lit);
        }

        m_context->IASetIndexBuffer(cfg.mesh->GetIndices(), DXGI_FORMAT_R32_UINT, 0);
        m_context->IASetVertexBuffers(0, cfg.mesh->GetVertexBufferCount(), cfg.mesh->GetVertexBuffers(), cfg.mesh->GetStrides(), cfg.mesh->GetOffsets());
        if (cfg.blend_factor != 1.0f) m_context->PSSetShaderResources(0, 1, cfg.texture->GetAddressOfSRV());

        m_context->DrawIndexed(cfg.mesh->GetIndexCount(), 0, 0);
    }

    void Renderer::SetAnisotropy(unsigned anisotropy)
    {
        Dragon_Check(anisotropy <= D3D11_REQ_MAXANISOTROPY);

        D3D11_SAMPLER_DESC desc{};
        desc.Filter = anisotropy ? D3D11_FILTER_ANISOTROPIC : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.MaxAnisotropy = anisotropy;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.BorderColor[0] = 1.0f; // NOTE: obnoxious pink border color
        desc.BorderColor[1] = 0.0f; // NOTE: obnoxious pink border color
        desc.BorderColor[2] = 1.0f; // NOTE: obnoxious pink border color
        desc.BorderColor[3] = 1.0f; // NOTE: obnoxious pink border color
        desc.MipLODBias = 0.0f;
        desc.MinLOD = 0.0f;
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        Dragon_CheckHR(m_device->CreateSamplerState(&desc, m_sampler_state_default.ReleaseAndGetAddressOf()));
    }
}
