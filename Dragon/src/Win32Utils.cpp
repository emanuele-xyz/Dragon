#include <Dragon/pch.h>
#include <Dragon/Win32Utils.h>
#include <Dragon/Commons.h>

#include <shlobj.h> // For SHBrowseForFolder and related functions
#include <commdlg.h> // For GetOpenFileName and related stuff
#include <shellapi.h> // For ShellExecute and related stuff

#include <scope_guard.hpp>

#include <imgui.h>
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Win32Utils
{
    LONGLONG GetPerformanceCounter()
    {
        LARGE_INTEGER perf{};
        Dragon_Check(QueryPerformanceCounter(&perf));
        return perf.QuadPart;
    }

    LONGLONG GetPerformanceFrequency()
    {
        LARGE_INTEGER freq{};
        Dragon_Check(QueryPerformanceFrequency(&freq));
        return freq.QuadPart;
    }

    float GetElapsedSec(LONGLONG t0, LONGLONG t1)
    {
        static LONGLONG frequency{ GetPerformanceFrequency() };
        float elapsed_sec{ static_cast<float>(t1 - t0) / static_cast<float>(frequency) };
        return elapsed_sec;
    }

    std::string OpenFolderDialog()
    {
        std::string res{};

        BROWSEINFO bi{};
        bi.lpszTitle = "Select a folder"; // Title of the dialog
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

        // Show the dialog
        LPITEMIDLIST pidl{ SHBrowseForFolder(&bi) };
        if (pidl)
        {
            auto free_pidl{ sg::make_scope_guard([&]() { CoTaskMemFree(pidl); }) };

            char path[MAX_PATH];
            // Get the selected folder's path
            if (SHGetPathFromIDList(pidl, path))
            {
                res = path;
            }
        }

        return res; // Return empty string if no folder was selected
    }

    std::string OpenFileDialog()
    {
        std::string res{};

        char path[MAX_PATH]{ '\0' };
        OPENFILENAME ofn{};
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = path;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = "Select a file";
        ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;

        // Display the dialog box
        if (GetOpenFileName(&ofn))
        {
            res = ofn.lpstrFile;
        }

        return res;
    }

    void OpenURLInBrowser(const std::string& url)
    {
        // Not a real HINSTANCE
        HINSTANCE result{ ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL) };
        // Check if the operation was successful
        if ((intptr_t)result <= 32) {
            // TODO: what if it fails?
        }
    }

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
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        {
            result = 1;
        }
        else
        {
            auto window{ reinterpret_cast<WindowHandle*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)) };
            result = window->OnMSG(hwnd, msg, wparam, lparam);
        }
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

    std::tuple<int, int> WindowHandle::GetClientDimensionsInt() const
    {
        RECT client_rect{};
        Dragon_Check(GetClientRect(m_handle, &client_rect));
        return { client_rect.right - client_rect.left, client_rect.bottom - client_rect.top };
    }

    std::tuple<float, float> WindowHandle::GetClientDimensionsFloat() const
    {
        auto [w, h] { GetClientDimensionsInt() };
        return { static_cast<float>(w), static_cast<float>(h) };
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