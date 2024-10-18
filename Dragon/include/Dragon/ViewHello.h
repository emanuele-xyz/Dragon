#pragma once
#include <Dragon/View.h>

#include <string>

namespace Dragon
{
    class ViewHello : public View
    {
    public:
        ViewHello(Win32Utils::WindowHandle& window, Gfx& gfx, ImGuiHandle& imgui, AppSettings& settings, AppData& data);
        ~ViewHello() override = default;
        ViewHello(const ViewHello&) = delete;
        ViewHello(ViewHello&&) noexcept = delete;
        ViewHello& operator=(const ViewHello&) = delete;
        ViewHello& operator=(ViewHello&&) noexcept = delete;
    public:
        std::unique_ptr<View> GetNextView() override;
    private:
        void OnWindowMSG(const MSG& msg) override;
        void Render() override;
    private:
        std::string m_project_path;
    };
}
