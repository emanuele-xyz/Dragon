#pragma once

#include <Dragon/ProjectSettings.h>
#include <Dragon/MeshManager.h>
#include <Dragon/TextureManager.h>
#include <Dragon/VertexShaderManager.h>
#include <Dragon/PixelShaderManager.h>

namespace Dragon
{
    struct ProjectResourceManagers
    {
        const MeshManager& mesh_mgr;
        const TextureManager& texture_mgr;
        const VertexShaderManager& vs_mgr;
        const PixelShaderManager& ps_mgr;
    };

    class Project
    {
    public:
        Project(const std::string& path, const ProjectResourceManagers& mgrs, bool is_new_project = false);
        ~Project() = default;
        Project(const Project&) = delete;
        Project(Project&&) noexcept = delete;
        Project& operator=(const Project&) = delete;
        Project& operator=(Project&&) noexcept = delete;
    private:
        ProjectSettings m_settings;
        const MeshManager& m_global_mesh_manager;
        const TextureManager& m_global_texture_manager;
        const VertexShaderManager& m_global_vertex_shader_manager;
        const PixelShaderManager& m_global_pixel_shader_manager;
        MeshManager m_mesh_manager;
        TextureManager m_texture_manager;
        VertexShaderManager m_vertex_shader_manager;
        PixelShaderManager m_pixel_shader_manager;
    };
}
