#include <Dragon/pch.h>
#include <Dragon/App.h>

#include <imgui.h>

namespace Dragon
{
    App::App()
        : m_is_running{ true }
        , m_window_class{ "dragon_window_class" }
        , m_window{ m_window_class.GetName(), "Dragon", 1280, 720, WS_OVERLAPPEDWINDOW }
        , m_gfx{ m_window.GetRawHandle() }
        , m_imgui{ m_window.GetRawHandle(), m_gfx.GetDevice(), m_gfx.GetContext() }
        , m_settings{}
        , m_data{}
    {
    }

    void App::Run()
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

            // NOTE: render scene
            {
                auto [client_w, client_h] { m_window.GetClientDimensionsFloat() };

                auto rtv{ m_gfx.GetBackBufferRTV() };
                auto dsv{ m_gfx.GetBackBufferDSV() };
                float clear_color[4]{ 1.0f, 0.0f, 1.0f, 1.0f };
                m_gfx.GetContext()->ClearRenderTargetView(rtv, clear_color);
                m_gfx.GetContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

                D3D11_VIEWPORT viewport{};
                viewport.TopLeftX = 0.0f;
                viewport.TopLeftY = 0.0f;
                viewport.Width = client_w;
                viewport.Height = client_h;
                viewport.MinDepth = 0.0f;
                viewport.MaxDepth = 1.0f;

                m_gfx.GetContext()->RSSetViewports(1, &viewport);
                //gfx.GetContext()->RSSetState(rasterizer_state.Get());
                m_gfx.GetContext()->OMSetRenderTargets(1, &rtv, dsv);
                //gfx.GetContext()->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
                m_gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                //gfx.GetContext()->IASetInputLayout(input_layout.Get());
                //gfx.GetContext()->VSSetShader(default_vs.Get(), nullptr, 0);
                //gfx.GetContext()->VSSetConstantBuffers(0, CONSTANT_BUFFERS_COUNT, constant_buffers);
                //gfx.GetContext()->PSSetConstantBuffers(0, CONSTANT_BUFFERS_COUNT, constant_buffers);
                //gfx.GetContext()->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
            }

            // NOTE: render ui
            m_imgui.NewFrame();
            {
                ImGui::Begin("App Settings");
                {
                    ImGui::Checkbox("V-Sync", &m_settings.vsync);
                }
                ImGui::End();

                ImGui::Begin("App Data");
                {
                    ImGui::Text("time since start (sec): %.1f", m_data.time_since_start_sec);
                    ImGui::Text("last frame dt (sec): %.6f", m_data.last_frame_dt_sec);
                    ImGui::Text("last frame dt (msec): %.3f", m_data.last_frame_dt_msec);
                    ImGui::Text("last fps: %.1f", m_data.last_fps);
                }
                ImGui::End();
            }
            m_imgui.Render();

            m_gfx.Present(m_settings.vsync);

            auto t1{ Win32Utils::GetPerformanceCounter() };

            m_data.last_frame_dt_sec = Win32Utils::GetElapsedSec(t0, t1);
            m_data.last_frame_dt_msec = m_data.last_frame_dt_sec * 1000.0f;
            m_data.time_since_start_sec += m_data.last_frame_dt_sec;
            m_data.last_fps = 1.0f / m_data.last_frame_dt_sec;
        }
    }
}
