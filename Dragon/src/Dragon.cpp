#include <Dragon/pch.h>
#include <Dragon/Dragon.h>
#include <Dragon/Win32Utils.h>
#include <Dragon/Gfx.h>

namespace Dragon
{
    void Run()
    {
        Win32Utils::WindowClass window_class{ "dragon_window_class" };
        Win32Utils::WindowHandle window{ window_class.GetName(), "Dragon", 1280, 720, WS_OVERLAPPEDWINDOW };
        Gfx gfx{ window.GetRawHandle() };

        bool is_running{ true };
        while (is_running)
        {
            window.ClearMessages();
            window.PumpMessages();

            // NOTE: process window messages
            {
                auto window_messages{ window.GetMessages() };
                for (const auto& msg : window_messages)
                {
                    if (msg.message == WM_CLOSE)
                    {
                        is_running = false;
                    } 
                    else if (msg.message == WM_SIZE)
                    {
                        gfx.Resize();
                    }
                }
            }

            // NOTE: render
            {
                auto rtv{ gfx.GetBackBufferRTV() };
                auto dsv{ gfx.GetBackBufferDSV() };
                float clear_color[4]{ 1.0f, 0.0f, 1.0f, 1.0f };
                gfx.GetContext()->ClearRenderTargetView(rtv, clear_color);
                gfx.GetContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
            }

            gfx.Present(false);
        }
    }
}
