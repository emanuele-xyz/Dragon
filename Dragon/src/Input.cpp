#include <Dragon/pch.h>
#include <Dragon/Input.h>

#include <imgui.h>

namespace Dragon
{
    static int KeyToVKey(Key key)
    {
        int vkey{};
        switch (key)
        {
        case Key::Backspace: { vkey = 0x08; } break;
        case Key::Tab: { vkey = 0x09; } break;
        case Key::Enter: { vkey = 0x0D; } break;
        case Key::Shift: { vkey = 0x10; } break;
        case Key::Ctrl: { vkey = 0x11; } break;
        case Key::Alt: { vkey = 0x12; } break;
        case Key::CapsLock: { vkey = 0x14; } break;
        case Key::Spacebar: { vkey = 0x20; } break;
        case Key::Left: { vkey = 0x25; } break;
        case Key::Up: { vkey = 0x26; } break;
        case Key::Right: { vkey = 0x27; } break;
        case Key::Down: { vkey = 0x28; } break;
        case Key::N0: { vkey = 0x30; } break;
        case Key::N1: { vkey = 0x31; } break;
        case Key::N2: { vkey = 0x32; } break;
        case Key::N3: { vkey = 0x33; } break;
        case Key::N4: { vkey = 0x34; } break;
        case Key::N5: { vkey = 0x35; } break;
        case Key::N6: { vkey = 0x36; } break;
        case Key::N7: { vkey = 0x37; } break;
        case Key::N8: { vkey = 0x38; } break;
        case Key::N9: { vkey = 0x39; } break;
        case Key::A: { vkey = 0x41; } break;
        case Key::B: { vkey = 0x42; } break;
        case Key::C: { vkey = 0x43; } break;
        case Key::D: { vkey = 0x44; } break;
        case Key::E: { vkey = 0x45; } break;
        case Key::F: { vkey = 0x46; } break;
        case Key::G: { vkey = 0x47; } break;
        case Key::H: { vkey = 0x48; } break;
        case Key::I: { vkey = 0x49; } break;
        case Key::J: { vkey = 0x4A; } break;
        case Key::K: { vkey = 0x4B; } break;
        case Key::L: { vkey = 0x4C; } break;
        case Key::M: { vkey = 0x4D; } break;
        case Key::N: { vkey = 0x4E; } break;
        case Key::O: { vkey = 0x4F; } break;
        case Key::P: { vkey = 0x50; } break;
        case Key::Q: { vkey = 0x51; } break;
        case Key::R: { vkey = 0x52; } break;
        case Key::S: { vkey = 0x53; } break;
        case Key::T: { vkey = 0x54; } break;
        case Key::U: { vkey = 0x55; } break;
        case Key::V: { vkey = 0x56; } break;
        case Key::W: { vkey = 0x57; } break;
        case Key::X: { vkey = 0x58; } break;
        case Key::Y: { vkey = 0x59; } break;
        case Key::Z: { vkey = 0x5A; } break;
        case Key::F1: { vkey = 0x70; } break;
        case Key::F2: { vkey = 0x71; } break;
        case Key::F3: { vkey = 0x72; } break;
        case Key::F4: { vkey = 0x73; } break;
        case Key::F5: { vkey = 0x74; } break;
        case Key::F6: { vkey = 0x75; } break;
        case Key::F7: { vkey = 0x76; } break;
        case Key::F8: { vkey = 0x77; } break;
        case Key::F9: { vkey = 0x78; } break;
        case Key::F10: { vkey = 0x79; } break;
        case Key::F11: { vkey = 0x7A; } break;
        case Key::F12: { vkey = 0x7B; } break;
        default: { Dragon_Unreachable(); } break;
        }
        return vkey;
    }

    static int KeyToVKey(int key)
    {
        return KeyToVKey(static_cast<Key>(key));
    }

    static int ButtonToVKey(Button button)
    {
        int vkey{};
        switch (button)
        {
        case Button::Left: { vkey = VK_LBUTTON; } break;
        case Button::Middle: { vkey = VK_MBUTTON; } break;
        case Button::Right: { vkey = VK_RBUTTON; } break;
        default: { Dragon_Unreachable(); } break;
        }
        return vkey;
    }

    static int ButtonToVKey(int button)
    {
        return ButtonToVKey(static_cast<Button>(button));
    }

    void Input::Update(std::span<const MSG> messages)
    {
        // TODO: this is unnecessary as of right now, but may be useful for input event generation and handling
        m_state_idx = (m_state_idx + 1) % 2;
        m_states[m_state_idx] = m_states[(m_state_idx + 1) % 2];
        m_states[m_state_idx].mouse.wheel = 0;

        // TODO: this could be implemented better
        if (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantTextInput)
        {
            for (int key{}; key < static_cast<int>(Key::Count); key++)
            {
                m_states[m_state_idx].keyboard.Set(key, false);
            }
        }
        else
        {
            for (int key{}; key < static_cast<int>(Key::Count); key++)
            {
                bool is_pressed{ GetKeyState(KeyToVKey(key)) < 0 };
                m_states[m_state_idx].keyboard.Set(key, is_pressed);
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
            m_states[m_state_idx].mouse.x = cursor_pos.x;
            m_states[m_state_idx].mouse.y = cursor_pos.y;

            for (int button{}; button < static_cast<int>(Button::Count); button++)
            {
                bool is_pressed{ GetKeyState(ButtonToVKey(button)) < 0 };
                m_states[m_state_idx].mouse.Set(button, is_pressed);
            }
        }

        for (const auto& msg : messages)
        {
            switch (msg.message)
            {
            case WM_MOUSEWHEEL:
            {
                if (!ImGui::GetIO().WantCaptureMouse)
                {
                    m_states[m_state_idx].mouse.wheel = GET_WHEEL_DELTA_WPARAM(msg.wParam);
                }
            } break;
            default: { /* NOTE: do nothing */ } break;
            }
        }
    }
}
