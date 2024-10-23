#pragma once

#include <Dragon/Win32Utils.h>
#include <Dragon/Gfx.h>
#include <Dragon/GfxResources.h>
#include <Dragon/Renderer.h>
#include <Dragon/ImGuiHandle.h>
#include <Dragon/AppSettings.h>
#include <Dragon/AppData.h>
#include <Dragon/MeshManager.h>
#include <Dragon/TextureManager.h>
#include <Dragon/VertexShaderManager.h>
#include <Dragon/PixelShaderManager.h>
#include <Dragon/Project.h>

#include <vector>
#include <string>
#include <memory>

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
        GfxResources m_gfx_resources;
        Renderer m_renderer;
        ImGuiHandle m_imgui;
        AppSettings m_settings;
        AppData m_data;
        MeshManager m_mesh_manager;
        TextureManager m_texture_manager;
        VertexShaderManager m_vertex_shader_manager;
        PixelShaderManager m_pixel_shader_manager;
        std::unique_ptr<Project> m_project;
        // ----------------------------
        bool m_show_message_window;
        std::string m_last_message;
        bool m_show_error_window;
        std::string m_last_error;
        bool m_show_new_project_window;
        char m_new_project_path[MAX_PATH];
        bool m_show_open_project_window;
        char m_open_project_path[MAX_PATH];
    };
}
