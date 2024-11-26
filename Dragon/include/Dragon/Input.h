#pragma once

#include <Dragon/Win32Utils.h>

#include <span>

namespace Dragon
{
    struct Keys
    {
        bool key[0xFF];
    };

    struct Curosr
    {
        int wheel;
        int x;
        int y;
    };

    struct InputState
    {
        Keys keys;
        Curosr cursor;
    };

    class Input
    {
    public:
        Input(HWND hwnd) : m_hwnd{hwnd}, m_state_idx {}, m_states{} {}
        ~Input() = default;
        Input(const Input&) = delete;
        Input(Input&&) noexcept = delete;
        Input& operator=(const Input&) = delete;
        Input& operator=(Input&&) noexcept = delete;
    public:
        void Update(std::span<const MSG> messages);
        const Keys& GetKeys() const { return m_states[m_state_idx].keys; }
        const Curosr& GetCursor() const { return m_states[m_state_idx].cursor; }
    private:
        HWND m_hwnd;
        int m_state_idx;
        InputState m_states[2];
    };
}
