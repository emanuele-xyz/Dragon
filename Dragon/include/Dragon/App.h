#pragma once

#include <Dragon/Win32Utils.h>
#include <Dragon/Gfx.h>
#include <Dragon/ImGuiHandle.h>
#include <Dragon/AppSettings.h>
#include <Dragon/AppData.h>

#include <string>

namespace Dragon
{
    class App
    {
    public:
        App();
        ~App() = default;
        App(const App&) = delete;
        App(App&&) noexcept = delete;
        App& operator=(const App&) = delete;
        App& operator=(App&&) noexcept = delete;
    public:
        void Run();
    private:
        bool m_is_running;
        Win32Utils::WindowClass m_window_class;
        Win32Utils::WindowHandle m_window;
        Gfx m_gfx;
        ImGuiHandle m_imgui;
        AppSettings m_settings;
        AppData m_data;
        // ----------------------------
        bool m_show_message_window;
        std::string m_last_message;
    };
}
