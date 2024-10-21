#include <Dragon/pch.h>
#include <Dragon/Renderer.h>

#include <Dragon/Math.h>
#include <Dragon/Mesh.h>

namespace Dragon
{
    struct CBCamera
    {
        Matrix view;
        Matrix projection;
    };

    struct CBObject
    {
        Matrix model;
        Matrix normal;
    };

    Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* context, GfxResources* gfx_resources)
        : m_device{ device }
        , m_context{ context }
        , m_gfx_resources{ gfx_resources }
    {
        // TODO: camera constants
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = sizeof(CBCamera);
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;
            Dragon_CheckHR(m_device->CreateBuffer(&desc, nullptr, m_cb_camera.ReleaseAndGetAddressOf()));
            m_constant_buffers[0] = m_cb_camera.Get();
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
            m_constant_buffers[1] = m_cb_object.Get();
        }
    }

    // TODO: dragon mesh and textures are just temporary!
    void Renderer::Render(float view_w, float view_h, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, GPUMesh* dragon_mesh, GPUTexture* dragon_texture)
    {
        float aspect{ view_w / view_h };

        // NOTE: update camera constants
        {
            D3D11Utils::SubresourceMapping subres_mapping{ m_context, m_cb_camera.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
            auto constants{ static_cast<CBCamera*>(subres_mapping.GetSubresource().pData) };
            constants->view = Matrix::CreateLookAt({ -1.0f, 2.0f, 2.0f }, Vector3::Zero, Vector3::Up);
            constants->projection = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(45.0f), aspect, 0.01f, 100.0f);
        }

        // NOTE: update base object constants
        {
            D3D11Utils::SubresourceMapping subres_mapping{ m_context, m_cb_object.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
            auto constants{ static_cast<CBObject*>(subres_mapping.GetSubresource().pData) };
            Matrix translate{ Matrix::CreateTranslation(Vector3::Zero) };
            Matrix rotate_x{ Matrix::CreateRotationX(DirectX::XMConvertToRadians(0.0f)) };
            Matrix rotate_y{ Matrix::CreateRotationY(DirectX::XMConvertToRadians(0.0f)) };
            Matrix rotate_z{ Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0.0f)) };
            Matrix rotate{ rotate_x * rotate_y * rotate_z };
            Matrix scale{ Matrix::CreateScale(Vector3::One) };
            Matrix model{ scale * rotate * translate };
            Matrix normal{ scale * rotate };
            normal.Invert();
            normal.Transpose();
            constants->model = model;
            constants->normal = normal;
        }

        float clear_color[4]{ 1.0f, 0.0f, 1.0f, 1.0f };
        m_context->ClearRenderTargetView(rtv, clear_color);
        m_context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

        D3D11_VIEWPORT viewport{};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = view_w;
        viewport.Height = view_h;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        m_context->RSSetViewports(1, &viewport);
        m_context->RSSetState(m_gfx_resources->GetRSDefault());
        m_context->OMSetRenderTargets(1, &rtv, dsv);
        m_context->OMSetDepthStencilState(m_gfx_resources->GetDSSDefault(), 0);
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_context->IASetIndexBuffer(dragon_mesh->indices.Get(), DXGI_FORMAT_R32_UINT, 0);
        m_context->IASetVertexBuffers(
            0, static_cast<UINT>(Mesh::VertexBufferIdx::Count),
            dragon_mesh->vertex_buffer_pointers, dragon_mesh->vertex_buffer_strides, dragon_mesh->vertex_buffer_offsets
        );
        m_context->IASetInputLayout(m_gfx_resources->GetInputLayout());
        m_context->VSSetShader(m_gfx_resources->GetVSDefault(), nullptr, 0);
        m_context->VSSetConstantBuffers(0, Dragon_CountOf(m_constant_buffers), m_constant_buffers);
        m_context->PSSetShader(m_gfx_resources->GetPSUnlit(), nullptr, 0);
        m_context->PSSetConstantBuffers(0, Dragon_CountOf(m_constant_buffers), m_constant_buffers);
        m_context->PSSetShaderResources(0, 1, dragon_texture->GetAddressOfSRV());
        {
            ID3D11SamplerState* sampler_states[]{ m_gfx_resources->GetSSDefault() };
            m_context->PSSetSamplers(0, Dragon_CountOf(sampler_states), sampler_states);
        }

        m_context->DrawIndexed(dragon_mesh->index_count, 0, 0);
    }
}