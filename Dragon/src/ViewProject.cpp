#include <Dragon/pch.h>
#include <Dragon/ViewProject.h>

namespace Dragon
{
    ViewProject::ViewProject(Win32Utils::WindowHandle& window, Gfx& gfx, ImGuiHandle& imgui, AppSettings& settings, AppData& data, const std::string& project_path)
        : View{ window, gfx, imgui, settings, data }
        , m_project_path{ project_path }
    {
    }

    void ViewProject::OnWindowMSG(const MSG& msg)
    {
        if (msg.message == WM_CLOSE)
        {
            // TODO: what if there are some unssaved project settings?
            m_is_running = false;
        }
        else if (msg.message == WM_SIZE)
        {
            m_gfx.Resize();
        }
    }

    void ViewProject::Render()
    {
        // TODO: to be implemented

        auto rtv{ m_gfx.GetBackBufferRTV() };
        auto dsv{ m_gfx.GetBackBufferDSV() };
        float clear_color[4]{ 0.0f, 1.0f, 1.0f, 1.0f };
        m_gfx.GetContext()->ClearRenderTargetView(rtv, clear_color);
        m_gfx.GetContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
        m_gfx.GetContext()->OMSetRenderTargets(1, &rtv, dsv);
    }
}
