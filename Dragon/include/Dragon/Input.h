#pragma once

#include <Dragon/Win32Utils.h>

#include <span>

namespace Dragon
{
    enum class Key
    {
        Backspace,
        Tab,
        Enter,
        Shift,
        Ctrl,
        Alt,
        CapsLock,
        Spacebar,
        Left,
        Up,
        Right,
        Down,
        N0,
        N1,
        N2,
        N3,
        N4,
        N5,
        N6,
        N7,
        N8,
        N9,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        Count
    };

    enum class Button
    {
        Left,
        Middle,
        Right,
        Count
    };

    struct Keyboard
    {
    public:
        bool Get(Key key) const { return keys[static_cast<int>(key)]; }
        bool Get(int key) const { return keys[key]; }
        void Set(Key key, bool val) { keys[static_cast<int>(key)] = val; }
        void Set(int key, bool val) { keys[key] = val; }
    private:
        bool keys[static_cast<int>(Key::Count)];
    };

    struct Mouse
    {
        int wheel;
        int x, y;
    public:
        bool Get(Button button) const { return buttons[static_cast<int>(button)]; }
        bool Get(int button) const { return buttons[button]; }
        void Set(Button button, bool val) { buttons[static_cast<int>(button)] = val; }
        void Set(int button, bool val) { buttons[button] = val; }
    private:
        bool buttons[static_cast<int>(Button::Count)];
    };

    struct InputState
    {
        Keyboard keyboard;
        Mouse mouse;
    };

    class Input
    {
    public:
        Input(HWND hwnd) : m_hwnd{ hwnd }, m_state_idx{}, m_states{} {}
        ~Input() = default;
        Input(const Input&) = delete;
        Input(Input&&) noexcept = delete;
        Input& operator=(const Input&) = delete;
        Input& operator=(Input&&) noexcept = delete;
    public:
        void Update(std::span<const MSG> messages);
        const Keyboard& GetKeyboard() const { return m_states[m_state_idx].keyboard; }
        const Mouse& GetMouse() const { return m_states[m_state_idx].mouse; }
    private:
        HWND m_hwnd;
        int m_state_idx;
        InputState m_states[2];
    };
}
