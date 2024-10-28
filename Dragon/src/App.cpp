#include <Dragon/pch.h>
#include <Dragon/App.h>

#include <imgui.h>

#include <Dragon/Math.h> // TODO: to be removed
#include <Dragon/Mesh.h> // TODO: to be removed?
#include <Dragon/Texture.h> // TODO: to be removed?

namespace Dragon
{
    App::App()
        : m_context{}
        , m_window_class{ "dragon_window_class" }
        , m_window{ m_window_class.GetName(), "Dragon", 1280, 720, WS_OVERLAPPEDWINDOW } // TODO: hardcoded
        , m_input{}
        , m_gfx{ m_window.GetRawHandle() }
        , m_imgui{ m_window.GetRawHandle(), m_gfx.GetDevice(), m_gfx.GetContext() }
        , m_renderer{ m_gfx.GetDevice(), m_gfx.GetContext() }
        , m_mesh_mgr{ m_gfx.GetDevice() }
        , m_texture_mgr{ m_gfx.GetDevice() }
    {
    }

    struct Object
    {
        Vector3 position{ Vector3::Zero };
        Quaternion rotation{ Quaternion::Identity };
        Vector3 scale{ Vector3::One };
        MeshRef mesh{};
        TextureRef texture{};
    };

    struct Camera
    {
        void ProcessInput(const KeyboardState& keyboard, const MouseState&, float dt)
        {
            Vector3 forward{ target - position };
            forward.y = 0; // project on xz plane
            Vector3 right{ forward.Cross(Vector3::Up) };

            if (keyboard.key['W'])
            {
                Move(forward * dt);
            }
            if (keyboard.key['S'])
            {
                Move(-forward * dt);
            }
            if (keyboard.key['A'])
            {
                Move(-right * dt);
            }
            if (keyboard.key['D'])
            {
                Move(right * dt);
            }
        }

        Matrix GetViewMatrix() const { return Matrix::CreateLookAt(position, target, Vector3::Up); }
        Matrix GetProjectionMatrix(float aspect) const { return Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(fov_deg), aspect, z_near, z_far); }

        void Move(Vector3 move) { position += move; target += move; }

        Vector3 position;
        Vector3 target;
        float fov_deg;
        float z_near;
        float z_far;
    };

    void App::Run()
    {
        Camera camera{};
        camera.position = { 10.0f, 10.f, 10.0f };
        camera.target = Vector3::Zero;
        camera.fov_deg = 45.0f;
        camera.z_near = 0.01f;
        camera.z_far = 100.0f;

        auto cube_ref{ m_mesh_mgr.Load("meshes/cube.obj") };
        auto plane_ref{ m_mesh_mgr.Load("meshes/plane.obj") };
        auto capsule_ref{ m_mesh_mgr.Load("meshes/capsule.obj") };

        auto lena_ref{ m_texture_mgr.Load("textures/lena.png") };
        auto proto_floor_ref{ m_texture_mgr.Load("textures/proto_floor.png") };

        std::vector<Object> objects{};
        {
            Object obj{};
            obj.position = { 0.0f, 1.0f, 0.0f };
            obj.mesh = cube_ref;
            obj.texture = lena_ref;
            objects.emplace_back(obj);
        }
        {
            Object obj{};
            obj.scale = { 20.0f, 1.0f, 20.0f };
            obj.mesh = plane_ref;
            obj.texture = proto_floor_ref;
            objects.emplace_back(obj);
        }
        {
            Object obj{};
            obj.position = { 3.0f, 2.0f, 0.0f };
            obj.mesh = capsule_ref;
            obj.texture = lena_ref;
            objects.emplace_back(obj);
        }

        while (m_context.is_running)
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
                        m_context.is_running = false;
                    }
                    else if (msg.message == WM_SIZE)
                    {
                        m_gfx.Resize();
                    }
                }

                m_input.Update(window_messages);
            }

            camera.ProcessInput(m_input.GetKeyboard(), m_input.GetMouse(), m_context.last_frame_dt_sec);

            // NOTE: render
            {
                auto rtv{ m_gfx.GetBackBufferRTV() };
                auto dsv{ m_gfx.GetBackBufferDSV() };
                auto [client_w, client_h] { m_window.GetClientDimensionsFloat() };

                m_renderer.NewFrame(rtv, dsv, client_w, client_h);

                // NOTE: update camera constants
                {
                    float aspect{ client_w / client_h };
                    auto view{ camera.GetViewMatrix() };
                    auto projection{ camera.GetProjectionMatrix(aspect) };
                    m_renderer.SetCamera(view, projection);
                }

                for (auto& obj : objects)
                {
                    m_renderer.Render(obj.position, obj.rotation, obj.scale, obj.mesh, obj.texture);
                }
            }

            // NOTE: render ui
            m_imgui.NewFrame();
            {
                ImGui::Begin("Graphics Settings");
                {
                    ImGui::Checkbox("V-Sync", &m_context.vsync);
                }
                ImGui::End();

                ImGui::Begin("Time Data");
                {
                    ImGui::Text("Time since start (sec): %.1f", m_context.time_since_start_sec);
                    ImGui::Text("Last frame dt (sec): %.6f", m_context.last_frame_dt_sec);
                    ImGui::Text("Last frame dt (msec): %.3f", m_context.last_frame_dt_msec);
                    ImGui::Text("Last FPS: %.1f", m_context.last_fps);
                }
                ImGui::End();

                ImGui::Begin("Mouse");
                {
                    const auto& mouse{ m_input.GetMouse() };
                    ImGui::Text("Wheel: %d", mouse.wheel);
                    ImGui::Text("Position: (%d,%d)", mouse.x, mouse.y);
                    ImGui::Text("LMR: %d%d%d", mouse.left, mouse.middle, mouse.right);
                }
                ImGui::End();

                ImGui::Begin("Keyboard");
                {
                    const auto& keyboard{ m_input.GetKeyboard() };
                    ImGui::Text("WASD: %d%d%d%d", keyboard.key['W'], keyboard.key['A'], keyboard.key['S'], keyboard.key['D']);
                }
                ImGui::End();
            }
            m_imgui.Render();

            m_gfx.Present(m_context.vsync);

            auto t1{ Win32Utils::GetPerformanceCounter() };

            m_context.last_frame_dt_sec = Win32Utils::GetElapsedSec(t0, t1);
            m_context.last_frame_dt_msec = m_context.last_frame_dt_sec * 1000.0f;
            m_context.time_since_start_sec += m_context.last_frame_dt_sec;
            m_context.last_fps = 1.0f / m_context.last_frame_dt_sec;
        }
    }
}
