#pragma once

#include <Dragon/AppContext.h>
#include <Dragon/Win32Utils.h>
#include <Dragon/Gfx.h>
#include <Dragon/GfxResources.h>
#include <Dragon/ImGuiHandle.h>

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
        AppContext m_context;
        Win32Utils::WindowClass m_window_class;
        Win32Utils::WindowHandle m_window;
        Gfx m_gfx;
        GfxResources m_gfx_resources;
        ImGuiHandle m_imgui;
    };
}
