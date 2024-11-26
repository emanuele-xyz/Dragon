#include <Dragon/pch.h>
#include <Dragon/Input.h>

#include <imgui.h>

namespace Dragon
{
    void Input::Update(std::span<const MSG> messages)
    {
        // TODO: this is unnecessary as of right now, but may be useful for input event generation and handling
        m_state_idx = (m_state_idx + 1) % 2;
        m_states[m_state_idx] = m_states[(m_state_idx + 1) % 2];
        m_states[m_state_idx].cursor.wheel = 0;

        // TODO: this could be implemented better
        if (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantTextInput)
        {
            for (int key{}; key < 0xFF; key++)
            {
                bool is_pressed{ false };
                m_states[m_state_idx].keys.key[key] = is_pressed;
            }
        }
        else
        {
            for (int key{}; key < 0xFF; key++)
            {
                bool is_pressed{ GetKeyState(key) < 0 };
                m_states[m_state_idx].keys.key[key] = is_pressed;
            }
        }

        if (ImGui::GetIO().WantCaptureMouse)
        {
            // NOTE: don't set cursor position to zero
        }
        else
        {
            POINT cursor_pos{};
            Dragon_Check(GetCursorPos(&cursor_pos));
            Dragon_Check(ScreenToClient(m_hwnd, &cursor_pos));
            m_states[m_state_idx].cursor.x = cursor_pos.x;
            m_states[m_state_idx].cursor.y = cursor_pos.y;
        }

        for (const auto& msg : messages)
        {
            switch (msg.message)
            {
            case WM_MOUSEWHEEL:
            {
                if (!ImGui::GetIO().WantCaptureMouse)
                {
                    m_states[m_state_idx].cursor.wheel = GET_WHEEL_DELTA_WPARAM(msg.wParam);
                }
            } break;
            default: { /* NOTE: do nothing */ } break;
            }
        }
    }
}
