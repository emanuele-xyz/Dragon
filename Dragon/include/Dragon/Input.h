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
        bool left;
        bool middle;
        bool right;
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
        void NewFrame();
        void Update(std::span<const MSG> messages);
        const KeyboardState& GetKeyboard() const { return m_states[m_state_idx].keyboard; }
        const MouseState& GetMouse() const { return m_states[m_state_idx].mouse; }
    private:
        int m_state_idx;
        InputState m_states[2];
    };
}
