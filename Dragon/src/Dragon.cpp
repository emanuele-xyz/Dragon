#include <Dragon/pch.h>
#include <Dragon/Dragon.h>
#include <Dragon/Win32Utils.h>

namespace Dragon
{
    void Run()
    {
        Win32Utils::WindowClass window_class{ "dragon_window_class" };
        Win32Utils::WindowHandle window{ window_class.GetName(), "Dragon", 1280, 720, WS_OVERLAPPEDWINDOW };

        bool is_running{ true };
        while (is_running)
        {
            window.ClearMessages();
            window.PumpMessages();
            auto window_messages{ window.GetMessages() };
            for (const auto& msg : window_messages)
            {
                if (msg.message == WM_CLOSE)
                {
                    is_running = false;
                }
            }
        }
    }
}
