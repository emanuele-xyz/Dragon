#pragma once

namespace Dragon
{
    struct AppContext
    {
        bool is_running{ true };
        bool vsync{ false };
        float time_since_start_sec;
        float last_frame_dt_sec;
        float last_frame_dt_msec;
        float last_fps;
    };
}
