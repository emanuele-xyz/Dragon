#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#include <wrl/client.h> // NOTE: for ComPtr
namespace wrl = Microsoft::WRL;

#include <vector>
#include <span>

namespace Win32Utils
{
    class WindowClass
    {
    public:
        static LRESULT Procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    public:
        WindowClass(const std::string& name);
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass(WindowClass&&) noexcept = delete;
        WindowClass& operator=(const WindowClass&) = delete;
        WindowClass& operator=(WindowClass&&) noexcept = delete;
    public:
        const std::string& GetName() const { return m_name; }
    private:
        std::string m_name;
    };

    class WindowHandle
    {
        friend class WindowClass;
    private:
        static LRESULT Procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    public:
        WindowHandle(const std::string& clss, const std::string& title, int client_w, int client_h, DWORD style);
        ~WindowHandle();
        WindowHandle(const WindowHandle&) = delete;
        WindowHandle(WindowHandle&&) noexcept = delete;
        WindowHandle& operator=(const WindowHandle&) = delete;
        WindowHandle& operator=(WindowHandle&&) noexcept = delete;
    public:
        void PumpMessages();
        std::span<const MSG> GetMessages() { return m_messages; }
        void ClearMessages() { m_messages.clear(); }
        HWND GetRawHandle() const { return m_handle; }
        std::tuple<int, int> GetClientDimensions() const;
    private:
        LRESULT OnMSG(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    private:
        HWND m_handle;
        std::vector<MSG> m_messages;
    };
}
