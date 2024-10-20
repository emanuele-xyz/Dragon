#pragma once

#include <string>
#include <vector>

namespace Dragon
{
    class ProjectSettings
    {
    public:
        ProjectSettings(const std::string& path, bool load_from_file = false);
        ~ProjectSettings() = default;
        ProjectSettings(const ProjectSettings&) = delete;
        ProjectSettings(ProjectSettings&&) noexcept = default;
        ProjectSettings& operator=(const ProjectSettings&) = delete;
        ProjectSettings& operator=(ProjectSettings&&) noexcept = default;
    public:
        void Save();
    private:
        std::string m_path;
        std::vector<std::string> m_asset_names;
        std::vector<std::string> m_scene_names;
    };
}
