#include <Dragon/pch.h>
#include <Dragon/App.h>

#include <imgui.h>

#include <Dragon/Math.h> // TODO: to be removed
#include <Dragon/CPUMesh.h> // TODO: to be removed
#include <Dragon/GPUMesh.h> // TODO: to be removed
#include <Dragon/CPUTexture.h> // TODO: to be removed
#include <Dragon/GPUTexture.h> // TODO: to be removed
#include <Dragon/ProjectSettings.h> // TODO: to be removed?

namespace Dragon
{
    App::App()
        : m_is_running{ true }
        , m_window_class{ "dragon_window_class" }
        , m_window{ m_window_class.GetName(), "Dragon", 1280, 720, WS_OVERLAPPEDWINDOW }
        , m_gfx{ m_window.GetRawHandle() }
        , m_gfx_resources{ m_gfx.GetDevice() }
        , m_renderer{ m_gfx.GetDevice(), m_gfx.GetContext(), &m_gfx_resources }
        , m_imgui{ m_window.GetRawHandle(), m_gfx.GetDevice(), m_gfx.GetContext() }
        , m_settings{}
        , m_data{}
        , m_mesh_manager{}
        , m_texture_manager{}
        , m_vertex_shader_manager{}
        , m_pixel_shader_manager{}
        , m_project{}
        , m_show_message_window{ true } // TODO: make false
        , m_last_message{ "Hello! This is a message sent from Dragon!" } // TODO: make empty
        , m_show_error_window{ true } // TODO: make false
        , m_last_error{ "Error! Something went wrong :c" } // TODO: make empty
        , m_show_new_project_window{ true } // TODO: make false
        , m_new_project_path{}
        , m_show_open_project_window{ true } // TODO: make false
        , m_open_project_path{}
    {
    }

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
                m_renderer.Render(client_w, client_h, m_gfx.GetBackBufferRTV(), m_gfx.GetBackBufferDSV(), &dragon_mesh, &dragon_texture);
            }

            // NOTE: render ui
            m_imgui.NewFrame();
            {
                if (ImGui::BeginMainMenuBar())
                {
                    if (ImGui::BeginMenu("Project"))
                    {
                        if (ImGui::MenuItem("New"))
                        {
                            m_show_new_project_window = true;
                        }
                        if (ImGui::MenuItem("Open"))
                        {
                            m_show_open_project_window = true;
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
                            auto path{ Win32Utils::BrowseForFile() };
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

                if (m_show_message_window)
                {
                    ImGui::PushStyleColor(ImGuiCol_TitleBg, { 0.0f, 0.3f, 0.0f, 1.0f });
                    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, { 0.0f, 0.5f, 0.0f, 1.0f });
                    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, { 0.0f, 0.3f, 0.0f, 0.5f });
                    if (ImGui::Begin("Message", &m_show_message_window))
                    {
                        ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, m_last_message.c_str());
                    }
                    ImGui::End();
                    ImGui::PopStyleColor(3);
                }

                if (m_show_error_window)
                {
                    ImGui::PushStyleColor(ImGuiCol_TitleBg, { 0.3f, 0.0f, 0.0f, 1.0f });
                    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, { 0.5f, 0.0f, 0.0f, 1.0f });
                    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, { 0.3f, 0.0f, 0.0f, 0.5f });
                    if (ImGui::Begin("Error", &m_show_error_window))
                    {
                        ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, m_last_error.c_str());
                    }
                    ImGui::End();
                    ImGui::PopStyleColor(3);
                }

                if (m_show_new_project_window)
                {
                    if (ImGui::Begin("New Project", &m_show_new_project_window))
                    {
                        ImGui::InputText("Path", m_new_project_path, MAX_PATH);
                        if (ImGui::Button("Create"))
                        {
                            if (std::filesystem::is_directory(m_new_project_path))
                            {
                                if (std::filesystem::is_empty(m_new_project_path))
                                {
                                    m_last_message = std::format("Creating new project in folder '{}'!", m_new_project_path);
                                    ProjectResourceManagers mgrs{ m_mesh_manager, m_texture_manager, m_vertex_shader_manager, m_pixel_shader_manager };
                                    m_project = std::make_unique<Project>(m_new_project_path, mgrs, true);
                                }
                                else
                                {
                                    m_last_error = std::format("Folder '{}' is not empty!", m_new_project_path);
                                }
                            }
                            else
                            {
                                m_last_error = std::format("'{}' is not a folder!", m_new_project_path);
                            }
                        }
                    }
                    ImGui::End();
                }

                if (m_show_open_project_window)
                {
                    if (ImGui::Begin("Open Project", &m_show_open_project_window))
                    {
                        ImGui::InputText("Path", m_open_project_path, MAX_PATH);
                        if (ImGui::Button("Open"))
                        {
                            auto path{ std::format("{}\\{}", m_open_project_path, DRAGON_PROJECT_FILE_NAME) };
                            if (std::filesystem::exists(path))
                            {
                                m_last_message = std::format("Opening project in folder '{}'!", m_open_project_path);
                                ProjectResourceManagers mgrs{ m_mesh_manager, m_texture_manager, m_vertex_shader_manager, m_pixel_shader_manager };
                                m_project = std::make_unique<Project>(m_open_project_path, mgrs);
                            }
                            else
                            {
                                m_last_error = std::format("There is no {} file in directory '{}'", DRAGON_PROJECT_FILE_NAME, m_open_project_path);
                            }
                        }
                    }
                    ImGui::End();
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

                ImGui::Begin("Textures");
                {
                    ImGui::Text("white");
                    auto white{ m_texture_manager.Get("white") };
                    ImGui::Image((ImTextureID)(intptr_t)white, ImVec2(64, 64));

                    ImGui::Text("black");
                    auto black{ m_texture_manager.Get("black") };
                    ImGui::Image((ImTextureID)(intptr_t)black, ImVec2(64, 64));

                    ImGui::Text("red");
                    auto red{ m_texture_manager.Get("red") };
                    ImGui::Image((ImTextureID)(intptr_t)red, ImVec2(64, 64));
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
    }
}
