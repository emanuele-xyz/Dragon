#pragma once

#include <Dragon/Win32Utils.h>

#include <span>

namespace Dragon
{
    struct KeyboardState
    {
        bool key[0xFF];
    };

    struct MouseState
    {
        int wheel;
        int x;
        int y;
        // TODO: add delta
        bool left;
        bool middle;
        bool right;
        // TODO: make buttons array + enum
        // TODO: make buttons action type array with enums nothing, press, release, hold DRAGON_ACTION_NONE, DRAGON_ACTION_PRESS, DRAGON_ACTION_RELEASE, DRAGON_ACTION_HOLD 
    };

    struct InputState
    {
        KeyboardState keyboard;
        MouseState mouse;
    };

    class Input
    {
    public:
        Input() : m_state_idx{}, m_states{} {}
        ~Input() = default;
        Input(const Input&) = delete;
        Input(Input&&) noexcept = delete;
        Input& operator=(const Input&) = delete;
        Input& operator=(Input&&) noexcept = delete;
    public:
        void Update(std::span<const MSG> messages);
        const KeyboardState& GetKeyboard() const { return m_states[m_state_idx].keyboard; }
        const MouseState& GetMouse() const { return m_states[m_state_idx].mouse; }
    private:
        int m_state_idx;
        InputState m_states[2];
    };
}
