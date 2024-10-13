#pragma once

#include <Dragon/Win32Utils.h>
#include <Dragon/Gfx.h>
#include <Dragon/ImGuiHandle.h>

namespace Dragon
{
    struct AppSettings
    {
        bool vsync{ false };
    };

    struct AppData
    {
        float time_since_start_sec;
        float last_frame_dt_sec;
        float last_frame_dt_msec;
        float last_fps;
    };

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
    };
}
