#include <Dragon/pch.h>
#include <Dragon/App.h>

#include <imgui.h>

#include <Dragon/Math.h> // TODO: to be removed
#include <Dragon/CPUMesh.h> // TODO: to be removed
#include <Dragon/GPUMesh.h> // TODO: to be removed
#include <Dragon/CPUTexture.h> // TODO: to be removed
#include <Dragon/GPUTexture.h> // TODO: to be removed
#include <Dragon/ProjectSettings.h> // TODO: to be removed?
#include <Dragon/ConstantBuffers.h> // TODO: to be removed?

namespace Dragon
{
    App::App()
        : m_is_running{ true }
        , m_gfx_settings{}
        , m_window_class{ "dragon_window_class" }
        , m_window{ m_window_class.GetName(), "Dragon", 1280, 720, WS_OVERLAPPEDWINDOW } // TODO: hardcoded
        , m_gfx{ m_window.GetRawHandle() }
        , m_gfx_resources{ m_gfx.GetDevice() }
        , m_imgui{ m_window.GetRawHandle(), m_gfx.GetDevice(), m_gfx.GetContext() }
        , m_time_data{}
        , m_mesh_manager{}
        , m_texture_manager{}
    {
    }

    struct ObjectDesc
    {
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;
        std::string mesh;
        std::string texture;
        bool is_lit;
    };

    struct LevelDesc
    {
        std::vector<ObjectDesc> objects;
    };

    void App::Run()
    {
        // NOTE: register default meshes
        m_mesh_manager.RegisterMesh(GPUMesh::FromCPUMesh(m_gfx.GetDevice(), CPUMesh::Cube()), "cube");

        // NOTE: register default textures
        m_texture_manager.RegisterTexture(GPUTexture::FromCPUTexture(m_gfx.GetDevice(), CPUTexture::Color(0, 0, 0, 255)), "black");
        m_texture_manager.RegisterTexture(GPUTexture::FromCPUTexture(m_gfx.GetDevice(), CPUTexture::Color(255, 255, 255, 255)), "white");
        m_texture_manager.RegisterTexture(GPUTexture::FromCPUTexture(m_gfx.GetDevice(), CPUTexture::Color(255, 0, 0, 255)), "red");

        GPUMesh dragon_mesh{};
        {
            auto tmp{ CPUMesh::LoadFromFile("../../assets/StanfordDragon.obj") };
            dragon_mesh = GPUMesh::FromCPUMesh(m_gfx.GetDevice(), tmp);
        }

        // TODO: use texture manager
        GPUTexture lena_texture{};
        {
            auto tmp{ CPUTexture::LoadFromFile("../../assets/lena.png") };
            lena_texture = GPUTexture::FromCPUTexture(m_gfx.GetDevice(), tmp);
        }
        GPUTexture dragon_texture{};
        {
            auto tmp{ CPUTexture::LoadFromFile("../../assets/StanfordDragon_albedo.jpeg") };
            dragon_texture = GPUTexture::FromCPUTexture(m_gfx.GetDevice(), tmp);
        }

        while (m_is_running)
        {
            auto t0{ Win32Utils::GetPerformanceCounter() };

            m_window.ClearMessages();
            m_window.PumpMessages();

            // NOTE: process window messages
            {
                auto window_messages{ m_window.GetMessages() };
                for (const auto& msg : window_messages)
                {
                    if (msg.message == WM_CLOSE)
                    {
                        m_is_running = false;
                    }
                    else if (msg.message == WM_SIZE)
                    {
                        m_gfx.Resize();
                    }
                }
            }

            auto [client_w, client_h] { m_window.GetClientDimensionsFloat() };

            // TODO: move to renderer
            // NOTE: render scene
            {
                float aspect{ client_w / client_h };

                // NOTE: update camera constants
                {
                    D3D11Utils::SubresourceMapping subres_mapping{ m_gfx.GetContext(), m_gfx_resources.GetCBCamera(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
                    auto constants{ static_cast<CBCamera*>(subres_mapping.GetSubresource().pData) };
                    constants->view = Matrix::CreateLookAt({ -1.0f, 2.0f, 2.0f }, Vector3::Zero, Vector3::Up);
                    constants->projection = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(45.0f), aspect, 0.01f, 100.0f);
                }

                // NOTE: update base object constants
                {
                    D3D11Utils::SubresourceMapping subres_mapping{ m_gfx.GetContext(), m_gfx_resources.GetCBObject(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
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

                auto rtv{ m_gfx.GetBackBufferRTV() };
                auto dsv{ m_gfx.GetBackBufferDSV() };

                float clear_color[4]{ 1.0f, 0.0f, 1.0f, 1.0f };
                m_gfx.GetContext()->ClearRenderTargetView(rtv, clear_color);
                m_gfx.GetContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

                D3D11_VIEWPORT viewport{};
                viewport.TopLeftX = 0.0f;
                viewport.TopLeftY = 0.0f;
                viewport.Width = client_w;
                viewport.Height = client_h;
                viewport.MinDepth = 0.0f;
                viewport.MaxDepth = 1.0f;

                m_gfx.GetContext()->RSSetViewports(1, &viewport);
                m_gfx.GetContext()->RSSetState(m_gfx_resources.GetRSDefault());
                m_gfx.GetContext()->OMSetRenderTargets(1, &rtv, dsv);
                m_gfx.GetContext()->OMSetDepthStencilState(m_gfx_resources.GetDSSDefault(), 0);
                m_gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                m_gfx.GetContext()->IASetIndexBuffer(dragon_mesh.indices.Get(), DXGI_FORMAT_R32_UINT, 0);
                m_gfx.GetContext()->IASetVertexBuffers(
                    0, static_cast<UINT>(Mesh::VertexBufferIdx::Count),
                    dragon_mesh.vertex_buffer_pointers, dragon_mesh.vertex_buffer_strides, dragon_mesh.vertex_buffer_offsets
                );
                m_gfx.GetContext()->IASetInputLayout(m_gfx_resources.GetInputLayout());
                m_gfx.GetContext()->VSSetShader(m_gfx_resources.GetVSDefault(), nullptr, 0);
                m_gfx.GetContext()->VSSetConstantBuffers(0, m_gfx_resources.GetConstantBuffersCount(), m_gfx_resources.GetConstantBuffers());
                m_gfx.GetContext()->PSSetShader(m_gfx_resources.GetPSUnlit(), nullptr, 0);
                m_gfx.GetContext()->PSSetConstantBuffers(0, m_gfx_resources.GetConstantBuffersCount(), m_gfx_resources.GetConstantBuffers());
                m_gfx.GetContext()->PSSetShaderResources(0, 1, dragon_texture.GetAddressOfSRV());
                {
                    ID3D11SamplerState* sampler_states[]{ m_gfx_resources.GetSSDefault() };
                    m_gfx.GetContext()->PSSetSamplers(0, Dragon_CountOf(sampler_states), sampler_states);
                }

                m_gfx.GetContext()->DrawIndexed(dragon_mesh.index_count, 0, 0);
            }

            // NOTE: render ui
            m_imgui.NewFrame();

            ImGui::Begin("Graphics Settings");
            {
                ImGui::Checkbox("V-Sync", &m_gfx_settings.vsync);
            }
            ImGui::End();

            ImGui::Begin("Time Data");
            {
                ImGui::Text("Time since start (sec): %.1f", m_time_data.time_since_start_sec);
                ImGui::Text("Last frame dt (sec): %.6f", m_time_data.last_frame_dt_sec);
                ImGui::Text("Last frame dt (msec): %.3f", m_time_data.last_frame_dt_msec);
                ImGui::Text("Last FPS: %.1f", m_time_data.last_fps);
            }
            ImGui::End();

            m_imgui.Render();

            m_gfx.Present(m_gfx_settings.vsync);

            auto t1{ Win32Utils::GetPerformanceCounter() };

            m_time_data.last_frame_dt_sec = Win32Utils::GetElapsedSec(t0, t1);
            m_time_data.last_frame_dt_msec = m_time_data.last_frame_dt_sec * 1000.0f;
            m_time_data.time_since_start_sec += m_time_data.last_frame_dt_sec;
            m_time_data.last_fps = 1.0f / m_time_data.last_frame_dt_sec;
        }
    }
}
