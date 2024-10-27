#include <Dragon/pch.h>
#include <Dragon/App.h>

#include <imgui.h>

#include <Dragon/Math.h> // TODO: to be removed
#include <Dragon/ConstantBuffers.h> // TODO: to be removed?
#include <Dragon/Mesh.h> // TODO: to be removed?
#include <Dragon/Texture.h> // TODO: to be removed?

namespace Dragon
{
    App::App()
        : m_context{}
        , m_window_class{ "dragon_window_class" }
        , m_window{ m_window_class.GetName(), "Dragon", 1280, 720, WS_OVERLAPPEDWINDOW } // TODO: hardcoded
        , m_input{}
        , m_gfx{ m_window.GetRawHandle() }
        , m_gfx_resources{ m_gfx.GetDevice() }
        , m_imgui{ m_window.GetRawHandle(), m_gfx.GetDevice(), m_gfx.GetContext() }
        , m_mesh_mgr{ m_gfx.GetDevice() }
        , m_texture_mgr{ m_gfx.GetDevice() }
    {
    }

    struct Object
    {
        Vector3 position{ Vector3::Zero };
        Quaternion rotation{ Quaternion::Identity };
        Vector3 scale{ Vector3::One };
        MeshRef mesh{};
        TextureRef texture{};
    };

    void App::Run()
    {
        auto cube_ref{ m_mesh_mgr.Load("meshes/cube.obj") };
        auto plane_ref{ m_mesh_mgr.Load("meshes/plane.obj") };
        auto capsule_ref{ m_mesh_mgr.Load("meshes/capsule.obj") };

        auto lena_ref{ m_texture_mgr.Load("textures/lena.png") };
        auto proto_floor_ref{ m_texture_mgr.Load("textures/proto_floor.png") };

        std::vector<Object> objects{};
        {
            Object obj{};
            obj.position = { 0.0f, 1.0f, 0.0f };
            obj.mesh = cube_ref;
            obj.texture = lena_ref;
            objects.emplace_back(obj);
        }
        {
            Object obj{};
            obj.scale = { 20.0f, 1.0f, 20.0f };
            obj.mesh = plane_ref;
            obj.texture = proto_floor_ref;
            objects.emplace_back(obj);
        }
        {
            Object obj{};
            obj.position = { 3.0f, 2.0f, 0.0f };
            obj.mesh = capsule_ref;
            obj.texture = lena_ref;
            objects.emplace_back(obj);
        }

        while (m_context.is_running)
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
                        m_context.is_running = false;
                    }
                    else if (msg.message == WM_SIZE)
                    {
                        m_gfx.Resize();
                    }
                }

                m_input.Update(window_messages);
            }

            auto [client_w, client_h] { m_window.GetClientDimensionsFloat() };

            // NOTE: render scene
            {
                float aspect{ client_w / client_h };

                // NOTE: update camera constants
                {
                    D3D11Utils::SubresourceMapping subres_mapping{ m_gfx.GetContext(), m_gfx_resources.GetCBCamera(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
                    auto constants{ static_cast<CBCamera*>(subres_mapping.GetSubresource().pData) };
                    constants->view = Matrix::CreateLookAt({ 10.0f, 10.f, 10.0f }, Vector3::Zero, Vector3::Up);
                    constants->projection = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(45.0f), aspect, 0.01f, 100.0f);
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
                m_gfx.GetContext()->IASetInputLayout(m_gfx_resources.GetInputLayout());
                m_gfx.GetContext()->VSSetShader(m_gfx_resources.GetVSDefault(), nullptr, 0);
                m_gfx.GetContext()->VSSetConstantBuffers(0, m_gfx_resources.GetConstantBuffersCount(), m_gfx_resources.GetConstantBuffers());
                m_gfx.GetContext()->PSSetShader(m_gfx_resources.GetPSUnlit(), nullptr, 0);
                m_gfx.GetContext()->PSSetConstantBuffers(0, m_gfx_resources.GetConstantBuffersCount(), m_gfx_resources.GetConstantBuffers());
                {
                    ID3D11SamplerState* sampler_states[]{ m_gfx_resources.GetSSDefault() };
                    m_gfx.GetContext()->PSSetSamplers(0, Dragon_CountOf(sampler_states), sampler_states);
                }

                for (auto& obj : objects)
                {
                    // NOTE: update object constants
                    {
                        D3D11Utils::SubresourceMapping subres_mapping{ m_gfx.GetContext(), m_gfx_resources.GetCBObject(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
                        auto constants{ static_cast<CBObject*>(subres_mapping.GetSubresource().pData) };
                        Matrix translate{ Matrix::CreateTranslation(obj.position) };
                        Matrix rotate{ Matrix::CreateFromQuaternion(obj.rotation) };
                        Matrix scale{ Matrix::CreateScale(obj.scale) };
                        Matrix model{ scale * rotate * translate };
                        Matrix normal{ scale * rotate };
                        normal.Invert();
                        normal.Transpose();
                        constants->model = model;
                        constants->normal = normal;
                    }

                    auto mesh{ obj.mesh };
                    auto texture{ obj.texture };

                    m_gfx.GetContext()->IASetIndexBuffer(mesh->GetIndices(), DXGI_FORMAT_R32_UINT, 0);
                    m_gfx.GetContext()->IASetVertexBuffers(0, mesh->GetVertexBufferCount(), mesh->GetVertexBuffers(), mesh->GetStrides(), mesh->GetOffsets());
                    m_gfx.GetContext()->PSSetShaderResources(0, 1, texture->GetAddressOfSRV());

                    m_gfx.GetContext()->DrawIndexed(mesh->GetIndexCount(), 0, 0);
                }
            }

            // NOTE: render ui
            m_imgui.NewFrame();
            {
                ImGui::Begin("Graphics Settings");
                {
                    ImGui::Checkbox("V-Sync", &m_context.vsync);
                }
                ImGui::End();

                ImGui::Begin("Time Data");
                {
                    ImGui::Text("Time since start (sec): %.1f", m_context.time_since_start_sec);
                    ImGui::Text("Last frame dt (sec): %.6f", m_context.last_frame_dt_sec);
                    ImGui::Text("Last frame dt (msec): %.3f", m_context.last_frame_dt_msec);
                    ImGui::Text("Last FPS: %.1f", m_context.last_fps);
                }
                ImGui::End();

                ImGui::Begin("Mouse");
                {
                    const auto& mouse{ m_input.GetMouse() };
                    ImGui::Text("Wheel: %d", mouse.wheel);
                    ImGui::Text("Position: (%d,%d)", mouse.x, mouse.y);
                    ImGui::Text("LMR: %d%d%d", mouse.left, mouse.middle, mouse.right);
                }
                ImGui::End();

                ImGui::Begin("Keyboard");
                {
                    const auto& keyboard{ m_input.GetKeyboard() };
                    ImGui::Text("WASD: %d%d%d%d", keyboard.key['W'], keyboard.key['A'], keyboard.key['S'], keyboard.key['D']);
                }
                ImGui::End();
            }
            m_imgui.Render();

            m_gfx.Present(m_context.vsync);

            auto t1{ Win32Utils::GetPerformanceCounter() };

            m_context.last_frame_dt_sec = Win32Utils::GetElapsedSec(t0, t1);
            m_context.last_frame_dt_msec = m_context.last_frame_dt_sec * 1000.0f;
            m_context.time_since_start_sec += m_context.last_frame_dt_sec;
            m_context.last_fps = 1.0f / m_context.last_frame_dt_sec;
        }
    }
}
