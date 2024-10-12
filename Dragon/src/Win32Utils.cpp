#include <Dragon/pch.h>
#include <Dragon/Win32Utils.h>
#include <Dragon/Commons.h>

namespace Win32Utils
{
    LRESULT WindowClass::Procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        LRESULT result{};
        if (msg == WM_CREATE)
        {
            // NOTE: setup window user data and window procedure
            auto create{ reinterpret_cast<CREATESTRUCT*>(lparam) };
            auto window{ static_cast<WindowHandle*>(create->lpCreateParams) };
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowHandle::Procedure));
            result = WindowHandle::Procedure(hwnd, msg, wparam, lparam);
        }
        else
        {
            result = DefWindowProc(hwnd, msg, wparam, lparam);
        }
        return result;
    }

    WindowClass::WindowClass(const std::string& name)
        : m_name{ name }
    {
        WNDCLASSEX wc{ 0 };
        wc.cbSize = sizeof(wc);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowClass::Procedure;
        //wc.cbClsExtra = ;
        //wc.cbWndExtra = ;
        wc.hInstance = GetModuleHandle(NULL);
        wc.hIcon = LoadIcon(0, IDI_APPLICATION);
        wc.hCursor = LoadCursor(0, IDC_ARROW);
        //wc.hbrBackground = ;
        //wc.lpszMenuName = ;
        wc.lpszClassName = name.c_str();
        wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

        Dragon_Check(RegisterClassEx(&wc));
    }

    WindowClass::~WindowClass()
    {
        UnregisterClass(m_name.c_str(), GetModuleHandle(NULL));
    }

    LRESULT WindowHandle::Procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        LRESULT result{};
        auto window{ reinterpret_cast<WindowHandle*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)) };
        result = window->OnMSG(hwnd, msg, wparam, lparam);
        return result;
    }

    WindowHandle::WindowHandle(const std::string& clss, const std::string& title, int client_w, int client_h, DWORD style)
        : m_handle{}
        , m_messages{}
    {
        int window_w{};
        int window_h{};
        {
            RECT rect{ 0, 0, client_w, client_h };
            Dragon_Check(AdjustWindowRect(&rect, style, FALSE));
            window_w = rect.right - rect.left;
            window_h = rect.bottom - rect.top;
        }

        Dragon_Check(m_handle = CreateWindow(clss.c_str(), title.c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT, window_w, window_h, 0, 0, GetModuleHandle(NULL), this));
        ShowWindow(m_handle, SW_SHOW);
    }

    void WindowHandle::PumpMessages()
    {
        MSG msg{};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    std::tuple<int, int> WindowHandle::GetClientDimensions() const
    {
        RECT client_rect{};
        Dragon_Check(GetClientRect(m_handle, &client_rect));
        return { client_rect.right - client_rect.left, client_rect.bottom - client_rect.top };
    }

    LRESULT WindowHandle::OnMSG(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        m_messages.push_back({ .hwnd = hwnd, .message = msg, .wParam = wparam, .lParam = lparam });

        LRESULT result{};
        if (msg == WM_CLOSE)
        {
            result = 0; // NOTE: from https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-close
        }
        else
        {
            result = DefWindowProc(hwnd, msg, wparam, lparam);
        }
        return result;
    }

    WindowHandle::~WindowHandle()
    {
        DestroyWindow(m_handle);
    }
}