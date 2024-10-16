#pragma once

#include <Dragon/Win32Utils.h>
#include <Dragon/Gfx.h>
#include <Dragon/ImGuiHandle.h>
#include <Dragon/AppSettings.h>
#include <Dragon/AppData.h>

namespace Dragon
{
    class View
    {
    public:
        View(Win32Utils::WindowHandle& window, Gfx& gfx, ImGuiHandle& imgui, AppSettings& settings, AppData& data);
        virtual ~View() {}
        View(const View&) = delete;
        View(View&&) noexcept = delete;
        View& operator=(const View&) = delete;
        View& operator=(View&&) noexcept = delete;
    public:
        virtual void Render() {}
    public:
        void Run();
    protected:
        bool m_is_running;
        Win32Utils::WindowHandle& m_window;
        Gfx& m_gfx;
        ImGuiHandle& m_imgui;
        AppSettings& m_settings;
        AppData& m_data;
    };
}
