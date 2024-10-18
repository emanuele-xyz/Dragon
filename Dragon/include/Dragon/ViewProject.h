#pragma once
#include <Dragon/View.h>

namespace Dragon
{
	// TODO: to be implemented
	class ViewProject : public View
	{
	public:
		ViewProject(Win32Utils::WindowHandle& window, Gfx& gfx, ImGuiHandle& imgui, AppSettings& settings, AppData& data, const std::string& project_path);
		~ViewProject() override = default;
		ViewProject(const ViewProject&) = delete;
		ViewProject(ViewProject&&) noexcept = delete;
		ViewProject& operator=(const ViewProject&) = delete;
		ViewProject& operator=(ViewProject&&) noexcept = delete;
	private:
		void OnWindowMSG(const MSG& msg) override;
		void Render() override;
	private:
		std::string m_project_path;
	};
}
