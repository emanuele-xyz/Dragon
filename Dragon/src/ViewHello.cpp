#include <Dragon/pch.h>
#include <Dragon/ViewHello.h>
#include <Dragon/ViewProject.h>

#include <imgui.h>

namespace Dragon
{
    ViewHello::ViewHello(Win32Utils::WindowHandle& window, Gfx& gfx, ImGuiHandle& imgui, AppSettings& settings, AppData& data)
        : View{ window, gfx, imgui, settings, data }
        , m_project_path{}
    {
    }

    std::unique_ptr<View> ViewHello::GetNextView()
    {
        if (m_project_path.empty())
        {
            return {};
        }
        else
        {
            return std::make_unique<ViewProject>(m_window, m_gfx, m_imgui, m_settings, m_data, m_project_path);
        }
    }

    void ViewHello::OnWindowMSG(const MSG& msg)
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

    void ViewHello::Render()
    {
        // NOTE: setup D3D11
        {
            auto rtv{ m_gfx.GetBackBufferRTV() };
            auto dsv{ m_gfx.GetBackBufferDSV() };
            float clear_color[4]{ 1.0f, 0.0f, 1.0f, 1.0f };
            m_gfx.GetContext()->ClearRenderTargetView(rtv, clear_color);
            m_gfx.GetContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
            m_gfx.GetContext()->OMSetRenderTargets(1, &rtv, dsv);
        }

        // NOTE: render ui
        m_imgui.NewFrame();
        {
            auto [client_w, client_h] { m_window.GetClientDimensionsFloat() };

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
                if (ImGui::Button("Open Project"))
                {
                    auto project_path{ Win32Utils::OpenFolderDialog() };
                    if (!project_path.empty())
                    {
                        // TODO: check whether there is a proper project file inside the folder
                        m_project_path = project_path;
                        m_is_running = false;
                    }
                }
                if (ImGui::Button("New Project"))
                {
                    auto path{ Win32Utils::BrowseForFolder() };
                    int kek{};
                    kek++;
                }

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
        m_imgui.Render();
    }
}