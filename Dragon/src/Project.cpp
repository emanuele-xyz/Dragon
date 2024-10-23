#include <Dragon/pch.h>
#include <Dragon/Project.h>

namespace Dragon
{
    Project::Project(const std::string& path, const ProjectResourceManagers& mgrs, bool is_new_project)
        : m_settings{ path, !is_new_project }
        , m_global_mesh_manager{ mgrs.mesh_mgr }
        , m_global_texture_manager{ mgrs.texture_mgr }
        , m_global_vertex_shader_manager{ mgrs.vs_mgr }
        , m_global_pixel_shader_manager{ mgrs.ps_mgr }
        , m_mesh_manager{}
        , m_texture_manager{}
        , m_vertex_shader_manager{}
        , m_pixel_shader_manager{}
    {
        // NOTE: if the project is new, we don't want to load a settings file, but we want to write to disk a default settings file
        // NOTE: if the project is not new, we want to load the settings file
        if (is_new_project) m_settings.Save();
    }
}
