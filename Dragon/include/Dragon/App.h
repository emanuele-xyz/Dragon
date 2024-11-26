#pragma once

#include <Dragon/AppContext.h>
#include <Dragon/Scheduler.h>
#include <Dragon/Win32Utils.h>
#include <Dragon/Input.h>
#include <Dragon/GfxDevice.h>
#include <Dragon/GfxSettings.h>
#include <Dragon/SwapChain.h>
#include <Dragon/ImGuiHandle.h>
#include <Dragon/Renderer.h>
#include <Dragon/MeshMgr.h>
#include <Dragon/TextureMgr.h>

#include <entt/entt.hpp> // TODO: to be removed?

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
        Scheduler m_scheduler;
        Win32Utils::WindowClass m_window_class;
        Win32Utils::WindowHandle m_window;
        GfxDevice m_gfx_device;
        GfxSettings m_gfx_settings;
        SwapChain m_swap_chain;
        ImGuiHandle m_imgui;
        Input m_input;
        Renderer m_renderer;
        MeshMgr m_mesh_mgr;
        TextureMgr m_texture_mgr;
        entt::registry m_registry;
    };
}
