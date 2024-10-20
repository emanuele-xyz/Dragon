#include <Dragon/pch.h>
#include <Dragon/Project.h>

namespace Dragon
{
    Project::Project(const std::string& path, bool is_new_project)
        : m_settings{ path, !is_new_project }
    {
        // NOTE: if the project is new, we don't want to load a settings file, but we want to write to disk a default settings file
        // NOTE: if the project is not new, we want to load the settings file
        if (is_new_project) m_settings.Save();
    }
}
