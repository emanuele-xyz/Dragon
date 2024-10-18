#include <Dragon/pch.h>
#include <Dragon/App.h>

#include <imgui.h>

#include <Dragon/Math.h> // TODO: to be removed
#include <Dragon/CPUMesh.h> // TODO: to be removed
#include <Dragon/GPUMesh.h> // TODO: to be removed
#include <Dragon/CPUTexture.h> // TODO: to be removed
#include <Dragon/GPUTexture.h> // TODO: to be removed
#include <Dragon/ViewHello.h> 

namespace Dragon
{
    App::App()
        : m_is_running{ true }
        , m_window_class{ "dragon_window_class" }
        , m_window{ m_window_class.GetName(), "Dragon", 1280, 720, WS_OVERLAPPEDWINDOW }
        , m_gfx{ m_window.GetRawHandle() }
        , m_imgui{ m_window.GetRawHandle(), m_gfx.GetDevice(), m_gfx.GetContext() }
        , m_settings{}
        , m_data{}
    {
    }

    void App::Run()
    {
        std::unique_ptr<View> current_view{ std::make_unique<ViewHello>(m_window, m_gfx, m_imgui, m_settings, m_data) };
        while (current_view)
        {
            current_view->Run();
            auto next_view{ current_view->GetNextView() };
            current_view = std::move(next_view);
        }

        return;

        #if 0
        // TODO: implement shader table
        auto [vs_default, vs_blob] {D3D11Utils::LoadVertexShaderFromFile(m_gfx.GetDevice(), "cso/DefaultVS.cso")};
        auto ps_unlit{ D3D11Utils::LoadPixelShaderFromFile(m_gfx.GetDevice(), "cso/UnlitPS.cso") };
        D3D11_INPUT_ELEMENT_DESC input_element_desc[]
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        wrl::ComPtr<ID3D11InputLayout> input_layout{
            D3D11Utils::CreateInputLayout(m_gfx.GetDevice(), input_element_desc, Dragon_CountOf(input_element_desc), vs_blob.Get())
        };

        // TODO: move to gfx
        wrl::ComPtr<ID3D11RasterizerState> rasterizer_state{};
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

            Dragon_CheckHR(m_gfx.GetDevice()->CreateRasterizerState(&desc, rasterizer_state.ReleaseAndGetAddressOf()));
        }

        // TODO: move to gfx
        wrl::ComPtr<ID3D11DepthStencilState> depth_stencil_state{};
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

            Dragon_CheckHR(m_gfx.GetDevice()->CreateDepthStencilState(&desc, depth_stencil_state.ReleaseAndGetAddressOf()));
        }

        // TODO: move to gfx
        wrl::ComPtr<ID3D11SamplerState> sampler_state{};
        {
            D3D11_SAMPLER_DESC desc{};
            desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
            desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
            desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
            desc.MipLODBias = 0.0f;
            desc.MaxAnisotropy = 0;
            desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            desc.BorderColor[0] = 1.0f; // NOTE: obnoxious pink border color
            desc.BorderColor[1] = 0.0f; // NOTE: obnoxious pink border color
            desc.BorderColor[2] = 1.0f; // NOTE: obnoxious pink border color
            desc.BorderColor[3] = 1.0f; // NOTE: obnoxious pink border color
            desc.MinLOD = 0.0f;
            desc.MaxLOD = D3D11_FLOAT32_MAX;

            Dragon_CheckHR(m_gfx.GetDevice()->CreateSamplerState(&desc, sampler_state.ReleaseAndGetAddressOf()));
        }

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

        // TODO: move to renderer
        wrl::ComPtr<ID3D11Buffer> camera_constants{};
        wrl::ComPtr<ID3D11Buffer> object_constants{};
        ID3D11Buffer* constant_buffers[2]{};
        {
            {
                D3D11_BUFFER_DESC desc{};
                desc.ByteWidth = sizeof(CBCamera);
                desc.Usage = D3D11_USAGE_DYNAMIC;
                desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                desc.MiscFlags = 0;
                desc.StructureByteStride = 0;
                Dragon_CheckHR(m_gfx.GetDevice()->CreateBuffer(&desc, nullptr, camera_constants.ReleaseAndGetAddressOf()));
                constant_buffers[0] = camera_constants.Get();
            }
            {
                D3D11_BUFFER_DESC desc{};
                desc.ByteWidth = sizeof(CBObject);
                desc.Usage = D3D11_USAGE_DYNAMIC;
                desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                desc.MiscFlags = 0;
                desc.StructureByteStride = 0;
                Dragon_CheckHR(m_gfx.GetDevice()->CreateBuffer(&desc, nullptr, object_constants.ReleaseAndGetAddressOf()));
                constant_buffers[1] = object_constants.Get();
            }
        }

        // TODO: use a mesh manager
        GPUMesh cube_mesh{};
        {
            auto tmp{ CPUMesh::LoadFromFile("../../assets/cube.obj") };
            cube_mesh = GPUMesh::FromCPUMesh(m_gfx.GetDevice(), tmp);
        }
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

            // TODO: move to renderer
            // NOTE: render scene
            {
                auto [client_w, client_h] { m_window.GetClientDimensionsFloat() };
                float aspect{ client_w / client_h };

                // NOTE: update camera constants
                // TODO: move to renderer
                {
                    {
                        D3D11Utils::SubresourceMapping subres_mapping{ m_gfx.GetContext(), camera_constants.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
                        auto constants{ static_cast<CBCamera*>(subres_mapping.GetSubresource().pData) };
                        constants->view = Matrix::CreateLookAt({ -1.0f, 2.0f, 2.0f }, Vector3::Zero, Vector3::Up);
                        constants->projection = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(45.0f), aspect, 0.01f, 100.0f);
                    }
                }

                // NOTE: update base object constants
                // TODO: move to renderer
                {
                    {
                        D3D11Utils::SubresourceMapping subres_mapping{ m_gfx.GetContext(), object_constants.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
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
                m_gfx.GetContext()->RSSetState(rasterizer_state.Get());
                m_gfx.GetContext()->OMSetRenderTargets(1, &rtv, dsv);
                m_gfx.GetContext()->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
                m_gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                m_gfx.GetContext()->IASetIndexBuffer(dragon_mesh.indices.Get(), DXGI_FORMAT_R32_UINT, 0);
                m_gfx.GetContext()->IASetVertexBuffers(
                    0, static_cast<UINT>(Mesh::VertexBufferIdx::Count),
                    dragon_mesh.vertex_buffer_pointers, dragon_mesh.vertex_buffer_strides, dragon_mesh.vertex_buffer_offsets
                );
                m_gfx.GetContext()->IASetInputLayout(input_layout.Get());
                m_gfx.GetContext()->VSSetShader(vs_default.Get(), nullptr, 0);
                m_gfx.GetContext()->VSSetConstantBuffers(0, Dragon_CountOf(constant_buffers), constant_buffers);
                m_gfx.GetContext()->PSSetShader(ps_unlit.Get(), nullptr, 0);
                m_gfx.GetContext()->PSSetConstantBuffers(0, Dragon_CountOf(constant_buffers), constant_buffers);
                m_gfx.GetContext()->PSSetShaderResources(0, 1, dragon_texture.GetAddressOfSRV());
                m_gfx.GetContext()->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

                m_gfx.GetContext()->DrawIndexed(dragon_mesh.index_count, 0, 0);
            }

            // NOTE: render ui
            m_imgui.NewFrame();
            {
                auto [client_w, client_h] { m_window.GetClientDimensionsFloat() };
                //float aspect{ client_w / client_h };

                // NOTE: draw hello window
                {
                    ImVec2 window_size{ client_w / 3.0f, client_h / 3.0f };
                    ImVec2 window_half_size{ window_size.x / 2.0f, window_size.y / 2.0f };
                    ImVec2 center{ client_w / 2.0f, client_h / 2.0f };
                    ImVec2 window_pos{ center.x - window_half_size.x, center.y - window_half_size.y };

                    ImGui::SetNextWindowPos(window_pos);
                    ImGui::SetNextWindowSize(window_size);
                    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Dark gray opaque
                    ImGui::SetNextWindowFocus();

                    ImGui::Begin("Welcome to Dragon!", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
                    {
                        ImGui::Columns(2);

                        ImGui::Text("Get Started");
                        ImGui::NextColumn();
                        ImGui::Text("Help");
                        ImGui::Separator();

                        ImGui::NextColumn();
                        ImGui::Button("Open Project");
                        ImGui::Button("New Project");

                        ImGui::NextColumn();
                        ImGui::Button("Quick Guide");
                        if (ImGui::Button("Github Repository"))
                        {
                            Win32Utils::OpenURLInBrowser("https://github.com/emanuele-xyz/Dragon");
                        }
                        ImGui::Button("Documentation");
                        ImGui::Button("File a Bug Report");

                        ImGui::Columns(1);
                    }
                    ImGui::End();

                    ImGui::PopStyleColor();
                }

                if (ImGui::BeginMainMenuBar())
                {
                    if (ImGui::BeginMenu("Project"))
                    {
                        if (ImGui::MenuItem("New"))
                        {
                        }
                        if (ImGui::MenuItem("Open"))
                        {
                            auto path{ Win32Utils::OpenFolderDialog() };
                            int kek{};
                            kek += 1;
                        }

                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Scene"))
                    {
                        if (ImGui::MenuItem("New"))
                        {
                        }
                        if (ImGui::MenuItem("Open"))
                        {
                            auto path{ Win32Utils::OpenFileDialog() };
                            int kek{};
                            kek += 1;
                        }
                        if (ImGui::MenuItem("Delete"))
                        {
                        }

                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Settings"))
                    {
                        if (ImGui::MenuItem("Graphics"))
                        {
                        }
                        if (ImGui::MenuItem("Camera"))
                        {
                        }

                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("View"))
                    {
                        if (ImGui::MenuItem("Data"))
                        {
                        }

                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Help"))
                    {
                        if (ImGui::MenuItem("Quick Guide"))
                        {
                        }
                        if (ImGui::MenuItem("Github Repository"))
                        {
                            Win32Utils::OpenURLInBrowser("https://github.com/emanuele-xyz/Dragon");
                        }
                        if (ImGui::MenuItem("Documentation"))
                        {
                        }
                        if (ImGui::MenuItem("File a Bug Report"))
                        {
                        }

                        ImGui::EndMenu();
                    }

                    ImGui::EndMainMenuBar();
                }

                ImGui::Begin("App Settings");
                {
                    ImGui::Checkbox("V-Sync", &m_settings.vsync);
                }
                ImGui::End();

                ImGui::Begin("App Data");
                {
                    ImGui::Text("time since start (sec): %.1f", m_data.time_since_start_sec);
                    ImGui::Text("last frame dt (sec): %.6f", m_data.last_frame_dt_sec);
                    ImGui::Text("last frame dt (msec): %.3f", m_data.last_frame_dt_msec);
                    ImGui::Text("last fps: %.1f", m_data.last_fps);
                }
                ImGui::End();
            }
            m_imgui.Render();

            m_gfx.Present(m_settings.vsync);

            auto t1{ Win32Utils::GetPerformanceCounter() };

            m_data.last_frame_dt_sec = Win32Utils::GetElapsedSec(t0, t1);
            m_data.last_frame_dt_msec = m_data.last_frame_dt_sec * 1000.0f;
            m_data.time_since_start_sec += m_data.last_frame_dt_sec;
            m_data.last_fps = 1.0f / m_data.last_frame_dt_sec;
        }
        #endif
    }
}
