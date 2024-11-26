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
        , m_gfx_device{}
        , m_gfx_settings{ m_gfx_device.GetDevice() }
        , m_swap_chain{ m_gfx_device.GetDevice(), m_gfx_device.GetContext(), m_window.GetRawHandle(), m_gfx_settings.msaa_samples[m_gfx_settings.msaa_index] }
        , m_imgui{ m_window.GetRawHandle(), m_gfx_device.GetDevice(), m_gfx_device.GetContext() }
        , m_input{ m_window.GetRawHandle() }
        , m_renderer{ m_gfx_device.GetDevice(), m_gfx_device.GetContext() }
        , m_mesh_mgr{ m_gfx_device.GetDevice() }
        , m_texture_mgr{ m_gfx_device.GetDevice() }
        , m_registry{}
    {
    }

    struct CTransform
    {
        Vector3 position{ Vector3::Zero };
        Quaternion rotation{ Quaternion::Identity };
        Vector3 scale{ Vector3::One };
    };

    struct CMesh
    {
        MeshRef mesh{};
    };

    struct CTexture
    {
        TextureRef texture{};
    };

    struct CCamera
    {
        Vector3 target{};
        float fov_deg{ 45.0f };
        float z_near{ 0.01f };
        float z_far{ 100.0f };
        Matrix view{ Matrix::Identity };
        Matrix projection{ Matrix::Identity };
    };

    struct CLighting
    {
        float ambient_strength{ 0.1f };
        Vector3 light_color{ Vector3::One };
        Vector3 light_direction{ Vector3::Down };
        Vector3 light_rotation{};
    };

    struct CSoldier
    {

    };

    struct CTarget
    {
        Vector3 target{};
    };

    void SysCamera(entt::registry& registry, const Keys& keys, const Curosr& cursor, float dt, float aspect)
    {
        auto view{ registry.view<CTransform, CCamera>() };
        auto e{ view.front() };
        auto [transform, camera] {view.get<CTransform, CCamera>(e)};

        // NOTE: horizontal movement
        {
            Vector3 forward{ camera.target - transform.position };
            forward.y = 0; // project on xz plane
            Vector3 right{ forward.Cross(Vector3::Up) };

            Vector3 move{};
            if (keys.key['W'])
            {
                move = forward * dt;
            }
            if (keys.key['S'])
            {
                move = -forward * dt;
            }
            if (keys.key['A'])
            {
                move = -right * dt;
            }
            if (keys.key['D'])
            {
                move = right * dt;
            }

            transform.position += move;
            camera.target += move;
        }

        // NOTE: rotate around target
        {
            float theta{};
            if (keys.key['Q'])
            {
                theta = -dt;
            }
            if (keys.key['E'])
            {
                theta = +dt;
            }

            Vector3 pos_target_space{ transform.position - camera.target };
            Matrix rotation{ Matrix::CreateRotationY(theta) };
            Vector3 rotated_pos_target_space{ Vector3::Transform(pos_target_space, rotation) };
            Vector3 new_pos{ rotated_pos_target_space + camera.target };
            transform.position = new_pos;
        }

        // NOTE: vertical movement
        if (cursor.wheel)
        {
            Vector3 move{ 0.0f, cursor.wheel > 0 ? +1.0f : -1.0f, 0.0f };
            transform.position += move;
            camera.target += move;
        }

        // NOTE: compute view and projection matrices
        camera.view = Matrix::CreateLookAt(transform.position, camera.target, Vector3::Up);
        camera.projection = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(camera.fov_deg), aspect, camera.z_near, camera.z_far);
    }

    void SysPickTarget(entt::registry& registry, const Keys& keys, const Curosr& cursor, float view_w, float view_h)
    {
        if (keys.key[VK_LBUTTON])
        {
            Vector3 ray_dir{};
            Vector3 ray_origin{};
            {
                auto view{ registry.view<CTransform, CCamera>() };
                auto e{ view.front() };
                auto& transform{ registry.get<CTransform>(e) };
                auto& camera{ registry.get<CCamera>(e) };

                Viewport v{ 0.0f, 0.0f, view_w, view_h };
                ray_origin = transform.position;
                ray_dir = MathUtils::GetRayFromMouse(cursor.x, cursor.y, v, camera.view, camera.projection);
            }

            auto e{ registry.view<CSoldier>().front() };
            auto& target{ registry.get_or_emplace<CTarget>(e) };
            target.target = MathUtils::IntersectRayPlane(ray_origin, ray_dir, Vector3::Zero, Vector3::Up);
        }
    }

    void SysFollowTarget(entt::registry& registry, float dt)
    {
        // TODO: not a good idea. Possible heap allocation every frame.
        std::vector<entt::entity> remove_target_from{};

        auto view{ registry.view<CTransform, CTarget, CSoldier>() };
        for (auto e : view)
        {
            auto [transform, target] {registry.get<CTransform, CTarget>(e)};

            Vector3 move{ target.target - Vector3{ transform.position.x, 0.0f, transform.position.z } };
            if (move.LengthSquared() < 0.1f)
            {
                remove_target_from.emplace_back(e);
            }
            else
            {
                transform.position += move * dt;
            }
        }

        for (auto e : remove_target_from)
        {
            registry.erase<CTarget>(e);
        }
    }

    void App::Run()
    {
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

        // NOTE: create camera
        {
            auto e{ m_registry.create() };
            auto& t{ m_registry.emplace<CTransform>(e) };
            t.position = { 10.0f, 10.f, 10.0f };
            auto& c{ m_registry.emplace<CCamera>(e) };
            c.target = Vector3::Zero;
        }

        // NOTE: create floor
        {
            auto e{ m_registry.create() };
            auto& transform{ m_registry.emplace<CTransform>(e) };
            transform.scale = Vector3::One * 10.0f;
            auto& m{ m_registry.emplace<CMesh>(e) };
            m.mesh = plane_ref;
            auto& t{ m_registry.emplace<CTexture>(e) };
            t.texture = proto_floor_ref;
        }

        // NOTE: create soldier
        {
            auto e{ m_registry.create() };
            auto& transform{ m_registry.emplace<CTransform>(e) };
            transform.position = { 0.0f, 1.0f, 0.0f };
            m_registry.emplace<CSoldier>(e);
            auto& m{ m_registry.emplace<CMesh>(e) };
            m.mesh = soldier_ref;
            auto& t{ m_registry.emplace<CTexture>(e) };
            t.texture = solder_albedo_ref;
        }

        // NOTE: create lighting
        {
            auto e{ m_registry.create() };
            m_registry.emplace<CLighting>(e);
        }

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

            auto [client_w, client_h] { m_window.GetClientDimensionsFloat() };
            float aspect{ client_w / client_h };

            SysCamera(m_registry, m_input.GetKeys(), m_input.GetCursor(), m_context.last_frame_dt_sec, aspect);
            SysPickTarget(m_registry, m_input.GetKeys(), m_input.GetCursor(), client_w, client_h);
            SysFollowTarget(m_registry, m_context.last_frame_dt_sec);

            // NOTE: render
            {
                auto rtv{ m_swap_chain.GetRTV() };
                auto dsv{ m_swap_chain.GetDSV() };

                m_renderer.NewFrame(rtv, dsv, client_w, client_h);

                // NOTE: update camera constants
                {
                    const auto& camera{ m_registry.get<CCamera>(m_registry.view<CCamera>().front()) };
                    m_renderer.SetCamera(camera.view, camera.projection);
                }

                // NOTE: update lighting constants
                {
                    const auto& lighting{ m_registry.get<CLighting>(m_registry.view<CLighting>().front()) };

                    auto quat{ Quaternion::CreateFromYawPitchRoll(lighting.light_rotation) };
                    auto conj{ quat };
                    conj.Conjugate();
                    Quaternion p{ lighting.light_direction, 0 };
                    auto rotated_p{ conj * p * quat };
                    m_renderer.SetLighting(lighting.ambient_strength, { rotated_p.x, rotated_p.y, rotated_p.z }, lighting.light_color);
                }

                // NOTE: render
                for (auto e : m_registry.view<CTransform, CMesh, CTexture>())
                {
                    const auto& [transform, mesh, texture] {m_registry.get<CTransform, CMesh, CTexture>(e)};
                    RenderCfg cfg{};
                    cfg.position = transform.position;
                    cfg.rotation = transform.rotation;
                    cfg.scaling = transform.scale;
                    cfg.mesh = mesh.mesh;
                    cfg.texture = texture.texture;
                    m_renderer.Render(cfg);
                }

                // NOTE: render directional light gizmo
                {
                    const auto& lighting{ m_registry.get<CLighting>(m_registry.view<CLighting>().front()) };
                    RenderCfg cfg{};
                    cfg.position = { 0.0f, 5.0f, 0.0f }; // TODO: find a better way to do it
                    cfg.rotation = Quaternion::CreateFromYawPitchRoll(lighting.light_rotation);
                    cfg.mesh = light_direction_ref;
                    cfg.is_lit = false;
                    cfg.color = lighting.light_color;
                    cfg.blend_factor = 1.0f;
                    m_renderer.Render(cfg);
                }

                // NOTE: render unit target gizmo
                for (auto e : m_registry.view<CTarget>())
                {
                    const auto& target{ m_registry.get<CTarget>(e) };
                    RenderCfg cfg{};
                    cfg.position = target.target;
                    cfg.scaling = Vector3::One * 0.5f;
                    cfg.mesh = icosphere_ref;
                    cfg.is_lit = false;
                    cfg.color = { 0.0f, 0.0f, 1.0f };
                    cfg.blend_factor = 1.0f;
                    m_renderer.Render(cfg);
                }

                // TODO: to be removed
                // NOTE: render AABB
                {
                    Vector3 p_min{ 1.0f, 2.0f, 5.0f };
                    Vector3 p_max{ p_min + Vector3{4.0f, 2.0f, 1.0f} };

                    auto keys{ m_input.GetKeys() };
                    auto cursor{ m_input.GetCursor() };

                    // NOTE: intersect mouse ray with AABB
                    MathUtils::RayAABBIntersection intersection{};
                    if (keys.key[VK_LBUTTON])
                    {
                        Vector3 ray_dir{};
                        Vector3 ray_origin{};
                        {
                            auto view{ m_registry.view<CTransform, CCamera>() };
                            auto e{ view.front() };
                            auto& transform{ m_registry.get<CTransform>(e) };
                            auto& camera{ m_registry.get<CCamera>(e) };

                            Viewport v{ 0.0f, 0.0f, client_w, client_h };
                            ray_origin = transform.position;
                            ray_dir = MathUtils::GetRayFromMouse(cursor.x, cursor.y, v, camera.view, camera.projection);
                        }
                        intersection = MathUtils::IntersectRayAABB(ray_origin, ray_dir, p_min, p_max);
                    }

                    RenderCfg cfg{};
                    cfg.scaling = Vector3::One * 0.2f;
                    cfg.mesh = icosphere_ref;
                    cfg.is_lit = false;
                    cfg.blend_factor = 1.0f;

                    cfg.position = p_min;
                    cfg.color = { 1.0f, 0.0f, 0.0f };
                    m_renderer.Render(cfg);

                    cfg.position = p_max;
                    cfg.color = { 0.0f, 1.0f, 0.0f };
                    m_renderer.Render(cfg);

                    if (intersection.does_intersect)
                    {
                        cfg.position = intersection.p_far;
                        cfg.color = { 1.0f, 1.0f, 0.0f };
                        m_renderer.Render(cfg);

                        cfg.position = intersection.p_close;
                        cfg.color = { 1.0f, 1.0f, 0.0f };
                        m_renderer.Render(cfg);
                    }

                    m_renderer.RenderAABB(p_min, p_max, Vector3::One); // TODO: to be removed
                }
            }

            // NOTE: render ui
            m_imgui.NewFrame();
            {
                ImGui::Begin("Light Settings");
                {
                    auto& lighting{ m_registry.get<CLighting>(m_registry.view<CLighting>().front()) };
                    ImGui::DragFloat("Ambient Strength", &lighting.ambient_strength, 0.01f, 0.0f, 1.0f);
                    ImGuiEx::ColorPicker3("Light Color", lighting.light_color);
                    ImGuiEx::DragFloat3("Light Rotation", lighting.light_rotation, 0.01f);
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

                ImGui::Begin("Cursor");
                {
                    const auto& cursor{ m_input.GetCursor() };
                    ImGui::Text("Wheel: %d", cursor.wheel);
                    ImGui::Text("Position: (%d,%d)", cursor.x, cursor.y);
                }
                ImGui::End();

                ImGui::Begin("Keys");
                {
                    const auto& keys{ m_input.GetKeys() };
                    ImGui::Text("WASD: %d%d%d%d", keys.key['W'], keys.key['A'], keys.key['S'], keys.key['D']);
                    ImGui::Text("LMR: %d%d%d", keys.key[VK_LBUTTON], keys.key[VK_MBUTTON], keys.key[VK_RBUTTON]);
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
