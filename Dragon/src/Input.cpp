#include <Dragon/pch.h>
#include <Dragon/Input.h>

namespace Dragon
{
    void Input::Update(std::span<const MSG> messages)
    {
        // TODO: this is unnecessary as of right now, but may be useful for input event generation and handling
        m_state_idx = (m_state_idx + 1) % 2;
        m_states[m_state_idx] = m_states[(m_state_idx + 1) % 2];
        m_states[m_state_idx].mouse.wheel = 0;

        for (const auto& msg : messages)
        {
            switch (msg.message)
            {
            case WM_MOUSEWHEEL: { m_states[m_state_idx].mouse.wheel = GET_WHEEL_DELTA_WPARAM(msg.wParam); } break;
            case WM_MOUSEMOVE: { m_states[m_state_idx].mouse.x = GET_X_LPARAM(msg.lParam); m_states[m_state_idx].mouse.y = GET_Y_LPARAM(msg.lParam); } break;
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP: { m_states[m_state_idx].mouse.left = (msg.message == WM_LBUTTONDOWN); } break;
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP: { m_states[m_state_idx].mouse.middle = (msg.message == WM_MBUTTONDOWN); } break;
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP: { m_states[m_state_idx].mouse.right = (msg.message == WM_RBUTTONDOWN); } break;
            case WM_KEYDOWN:
            case WM_KEYUP: { m_states[m_state_idx].keyboard.key[static_cast<int>(msg.wParam)] = (msg.message == WM_KEYDOWN); } break;
            }
        }
    }
}
