#include <Dragon/pch.h>
#include <Dragon/Dragon.h>
#include <Dragon/Win32Utils.h>
#include <Dragon/Gfx.h>
#include <Dragon/ImGuiHandle.h>

#include <imgui.h>

namespace Dragon
{
    void Run()
    {
        Win32Utils::WindowClass window_class{ "dragon_window_class" };
        Win32Utils::WindowHandle window{ window_class.GetName(), "Dragon", 1280, 720, WS_OVERLAPPEDWINDOW };
        Gfx gfx{ window.GetRawHandle() };
        ImGuiHandle imgui_handle{ window.GetRawHandle(), gfx.GetDevice(), gfx.GetContext() };

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

            // NOTE: render scene
            {
                auto [client_w, client_h] { window.GetClientDimensionsFloat() };

                auto rtv{ gfx.GetBackBufferRTV() };
                auto dsv{ gfx.GetBackBufferDSV() };
                float clear_color[4]{ 1.0f, 0.0f, 1.0f, 1.0f };
                gfx.GetContext()->ClearRenderTargetView(rtv, clear_color);
                gfx.GetContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

                D3D11_VIEWPORT viewport{};
                viewport.TopLeftX = 0.0f;
                viewport.TopLeftY = 0.0f;
                viewport.Width = client_w;
                viewport.Height = client_h;
                viewport.MinDepth = 0.0f;
                viewport.MaxDepth = 1.0f;

                gfx.GetContext()->RSSetViewports(1, &viewport);
                //gfx.GetContext()->RSSetState(rasterizer_state.Get());
                gfx.GetContext()->OMSetRenderTargets(1, &rtv, dsv);
                //gfx.GetContext()->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
                gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                //gfx.GetContext()->IASetInputLayout(input_layout.Get());
                //gfx.GetContext()->VSSetShader(default_vs.Get(), nullptr, 0);
                //gfx.GetContext()->VSSetConstantBuffers(0, CONSTANT_BUFFERS_COUNT, constant_buffers);
                //gfx.GetContext()->PSSetConstantBuffers(0, CONSTANT_BUFFERS_COUNT, constant_buffers);
                //gfx.GetContext()->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
            }

            // NOTE: render ui
            imgui_handle.NewFrame();
            {
                ImGui::Begin("Hello!");
                {
                    ImGui::Text("Skibidi Toilet!");
                }
                ImGui::End();
            }
            imgui_handle.Render();

            gfx.Present(false);
        }
    }
}
