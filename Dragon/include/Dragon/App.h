#pragma once

#include <Dragon/Win32Utils.h>
#include <Dragon/Gfx.h>
#include <Dragon/GfxResources.h>
#include <Dragon/ImGuiHandle.h>
#include <Dragon/MeshManager.h>
#include <Dragon/TextureManager.h>

namespace Dragon
{
    struct GraphicsSettings
    {
        bool vsync{ false };
    };

    struct TimeData
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
        GraphicsSettings m_gfx_settings;
        Win32Utils::WindowClass m_window_class;
        Win32Utils::WindowHandle m_window;
        Gfx m_gfx;
        GfxResources m_gfx_resources;
        ImGuiHandle m_imgui;
        TimeData m_time_data;
        MeshManager m_mesh_manager;
        TextureManager m_texture_manager;
    };
}
