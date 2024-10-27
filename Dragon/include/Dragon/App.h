#pragma once

#include <Dragon/AppContext.h>
#include <Dragon/Win32Utils.h>
#include <Dragon/Input.h>
#include <Dragon/Gfx.h>
#include <Dragon/ImGuiHandle.h>
#include <Dragon/Renderer.h>
#include <Dragon/MeshMgr.h>
#include <Dragon/TextureMgr.h>

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
        Input m_input;
        Gfx m_gfx;
        ImGuiHandle m_imgui;
        Renderer m_renderer;
        MeshMgr m_mesh_mgr;
        TextureMgr m_texture_mgr;
    };
}
