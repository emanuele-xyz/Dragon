#include <Dragon/pch.h>
#include <Dragon/App.h>

#include <imgui.h>
#include <Dragon/ImGuiEx.h>

#include <Dragon/MathUtils.h> // TODO: to be removed
#include <Dragon/Mesh.h> // TODO: to be removed?
#include <Dragon/Texture.h> // TODO: to be removed?

namespace Dragon
{
    App::App()
        : m_context{}
        , m_scheduler{}
        , m_window_class{ "dragon_window_class" }
        , m_window{ m_window_class.GetName(), "Dragon", 1280, 720, WS_OVERLAPPEDWINDOW } // TODO: hardcoded
        , m_input{}
        , m_gfx_device{}
        , m_gfx_settings{ m_gfx_device.GetDevice() }
        , m_swap_chain{ m_gfx_device.GetDevice(), m_gfx_device.GetContext(), m_window.GetRawHandle(), m_gfx_settings.msaa_samples[m_gfx_settings.msaa_index] }
        , m_imgui{ m_window.GetRawHandle(), m_gfx_device.GetDevice(), m_gfx_device.GetContext() }
        , m_renderer{ m_gfx_device.GetDevice(), m_gfx_device.GetContext() }
        , m_mesh_mgr{ m_gfx_device.GetDevice() }
        , m_texture_mgr{ m_gfx_device.GetDevice() }
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
        void ProcessInput(const KeyboardState& keyboard, const MouseState& mouse, float dt)
        {
            // NOTE: horizontal movement
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

            // NOTE: rotate around target
            {
                if (keyboard.key['Q'])
                {
                    RotateAroundTarget(-dt);
                }
                if (keyboard.key['E'])
                {
                    RotateAroundTarget(+dt);
                }
            }

            // NOTE: vertical movement
            if (mouse.wheel)
            {
                Move({ 0.0f, mouse.wheel > 0 ? +1.0f : -1.0f, 0.0f });
            }
        }

        Matrix GetViewMatrix() const { return Matrix::CreateLookAt(position, target, Vector3::Up); }
        Matrix GetProjectionMatrix(float aspect) const { return Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(fov_deg), aspect, z_near, z_far); }

        void Move(Vector3 move) { position += move; target += move; }
        void RotateAroundTarget(float theta)
        {
            Vector3 pos_target_space{ position - target };
            Matrix rotation{ Matrix::CreateRotationY(theta) };
            Vector3 rotated_pos_target_space{ Vector3::Transform(pos_target_space, rotation) };
            Vector3 new_pos{ rotated_pos_target_space + target };
            position = new_pos;
        }

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
        auto light_direction_ref{ m_mesh_mgr.Load("meshes/light_direction.obj") };
        auto icosphere_ref{ m_mesh_mgr.Load("meshes/icosphere.obj") };
        auto soldier_ref{ m_mesh_mgr.Load("meshes/proto_soldier.obj") };

        auto lena_ref{ m_texture_mgr.Load("textures/lena.png") };
        auto proto_floor_ref{ m_texture_mgr.Load("textures/proto_floor.png") };
        auto paving_stones_ref{ m_texture_mgr.Load("textures/paving_stones.png") };
        auto solder_albedo_ref{ m_texture_mgr.Load("textures/proto_soldier.png") };
        auto red_ref{ m_texture_mgr.Load("textures/red.png") };
        auto blue_ref{ m_texture_mgr.Load("textures/blue.png") };

        std::vector<Object> objects{};
        {
            Object obj{};
            obj.position = { 0.0f, 2.0f, 0.0f };
            obj.rotation = Quaternion::CreateFromYawPitchRoll({ 45.0f, 0.0f, 0.0f });
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
        {
            Object obj{};
            obj.position = { 3.0f, 1.0f, 3.0f };
            obj.mesh = soldier_ref;
            obj.texture = solder_albedo_ref;
            objects.emplace_back(obj);
        }

        // NOTE: lighting data
        Vector3 ambient_color{ 0.1f, 0.1f, 0.1f };
        Vector3 light_color{ Vector3::One };
        Vector3 light_direction{ Vector3::Down };
        Vector3 light_rotation{};

        Vector3 unit_target{};

        while (m_context.is_running)
        {
            auto t0{ Win32Utils::GetPerformanceCounter() };

            m_scheduler.RunFrameStartCallbacks();

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
                        m_swap_chain.Resize();
                    }
                }

                m_input.Update(window_messages);
            }

            camera.ProcessInput(m_input.GetKeyboard(), m_input.GetMouse(), m_context.last_frame_dt_sec);

            // NOTE: make unit follow the target
            {
                const auto& mouse{ m_input.GetMouse() };
                if (mouse.left)
                {
                    int mouse_x{ mouse.x };
                    int mouse_y{ mouse.y };
                    auto view{ camera.GetViewMatrix() };
                    auto [client_w, client_h] { m_window.GetClientDimensionsFloat() };
                    Viewport v{ 0.0f, 0.0f, client_w, client_h };
                    Vector3 ray_dir{ MathUtils::GetRayFromMouse(mouse_x, mouse_y, v, camera.GetViewMatrix(), camera.GetProjectionMatrix(client_w / client_h)) };
                    Vector3 ray_origin{ camera.position };
                    unit_target = MathUtils::IntersectRayPlane(ray_origin, ray_dir, Vector3::Zero, Vector3::Up);
                }
            }

            // NOTE: render
            {
                auto rtv{ m_swap_chain.GetRTV() };
                auto dsv{ m_swap_chain.GetDSV() };
                auto [client_w, client_h] { m_window.GetClientDimensionsFloat() };

                m_renderer.NewFrame(rtv, dsv, client_w, client_h);

                // NOTE: update camera constants
                {
                    float aspect{ client_w / client_h };
                    auto view{ camera.GetViewMatrix() };
                    auto projection{ camera.GetProjectionMatrix(aspect) };
                    m_renderer.SetCamera(view, projection);
                }

                // NOTE: update lighting constants
                {
                    auto quat{ Quaternion::CreateFromYawPitchRoll(light_rotation) };
                    auto conj{ quat };
                    conj.Conjugate();
                    Quaternion p{ light_direction, 0 };
                    auto rotated_p{ conj * p * quat };
                    m_renderer.SetLighting(ambient_color, { rotated_p.x, rotated_p.y, rotated_p.z }, light_color);
                }

                for (auto& obj : objects)
                {
                    m_renderer.Render(obj.position, obj.rotation, obj.scale, obj.mesh, obj.texture);
                }

                // NOTE: render directional light gizmo
                m_renderer.Render({ 0.0f, 5.0f, 0.0f }, Quaternion::CreateFromYawPitchRoll(light_rotation), Vector3::One, light_direction_ref, lena_ref);

                // NOTE: render unit target
                m_renderer.Render(unit_target, Quaternion::Identity, Vector3::One * 0.5f, icosphere_ref, blue_ref);
            }

            // NOTE: render ui
            m_imgui.NewFrame();
            {
                ImGui::Begin("Light Settings");
                {
                    ImGuiEx::ColorPicker3("Ambient Color", ambient_color);
                    ImGuiEx::ColorPicker3("Light Color", light_color);
                    ImGuiEx::DragFloat3("Light Rotation", light_rotation, 0.01f);
                }
                ImGui::End();

                ImGui::Begin("Graphics Settings");
                {
                    ImGui::Checkbox("V-Sync", &m_gfx_settings.vsync);

                    if (ImGui::BeginListBox("MSAA"))
                    {
                        for (size_t i{}; i < m_gfx_settings.msaa_descriptions.size(); i++)
                        {
                            bool is_selected{ i == m_gfx_settings.msaa_index };
                            if (ImGui::Selectable(m_gfx_settings.msaa_descriptions[i].c_str(), is_selected))
                            {
                                m_scheduler.ScheduleAtNextFrameStart([i, this]() {
                                    m_gfx_settings.msaa_index = i;
                                    m_swap_chain.SetMSAASampleCount(m_gfx_settings.msaa_samples[m_gfx_settings.msaa_index]);
                                    });
                            }
                        }
                        ImGui::EndListBox();
                    }

                    if (ImGui::BeginListBox("Anisotropic Filtering"))
                    {
                        for (size_t i{}; i < m_gfx_settings.anisotropy_descriptions.size(); i++)
                        {
                            bool is_selected{ i == m_gfx_settings.anisotropy_index };
                            if (ImGui::Selectable(m_gfx_settings.anisotropy_descriptions[i].c_str(), is_selected))
                            {
                                m_scheduler.ScheduleAtNextFrameStart([i, this]() {
                                    m_gfx_settings.anisotropy_index = i;
                                    m_renderer.SetAnisotropy(m_gfx_settings.anisotropy_levels[m_gfx_settings.anisotropy_index]);
                                    });
                            }
                        }
                        ImGui::EndListBox();
                    }
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

            m_swap_chain.Present(m_gfx_settings.vsync);

            auto t1{ Win32Utils::GetPerformanceCounter() };

            m_context.last_frame_dt_sec = Win32Utils::GetElapsedSec(t0, t1);
            m_context.last_frame_dt_msec = m_context.last_frame_dt_sec * 1000.0f;
            m_context.time_since_start_sec += m_context.last_frame_dt_sec;
            m_context.last_fps = 1.0f / m_context.last_frame_dt_sec;
        }
    }
}
