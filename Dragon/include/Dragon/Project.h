#pragma once

#include <Dragon/ProjectSettings.h>

namespace Dragon
{
	class Project
	{
	public:
		Project(const std::string& path, bool is_new_project = false);
		~Project() = default;
		Project(const Project&) = delete;
		Project(Project&&) noexcept = delete;
		Project& operator=(const Project&) = delete;
		Project& operator=(Project&&) noexcept = delete;
	private:
		ProjectSettings m_settings;
	};
}
