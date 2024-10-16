#include <Dragon/pch.h>
#include <Dragon/View.h>
#include <Dragon/Win32Utils.h>

#include <imgui.h>

namespace Dragon
{
    View::View(Win32Utils::WindowHandle& window, Gfx& gfx, ImGuiHandle& imgui, AppSettings& settings, AppData& data)
        : m_is_running{ true }
        , m_window{ window }
        , m_gfx{ gfx }
        , m_imgui{ imgui }
        , m_settings{ settings }
        , m_data{ data }
    {
    }

    void View::Run()
    {
        while (m_is_running)
        {
            auto t0{ Win32Utils::GetPerformanceCounter() };

            m_window.ClearMessages();
            m_window.PumpMessages();

            // NOTE: process window messages
            {
                auto window_messages{ m_window.GetMessages() };
                for (const auto& msg : window_messages)
                {
                    if (msg.message == WM_CLOSE)
                    {
                        m_is_running = false;
                    }
                    else if (msg.message == WM_SIZE)
                    {
                        m_gfx.Resize();
                    }
                }
            }

            Render();

            m_gfx.Present(m_settings.vsync);

            auto t1{ Win32Utils::GetPerformanceCounter() };

            m_data.last_frame_dt_sec = Win32Utils::GetElapsedSec(t0, t1);
            m_data.last_frame_dt_msec = m_data.last_frame_dt_sec * 1000.0f;
            m_data.time_since_start_sec += m_data.last_frame_dt_sec;
            m_data.last_fps = 1.0f / m_data.last_frame_dt_sec;
        }
    }
}