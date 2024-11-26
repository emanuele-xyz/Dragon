#pragma once

#include <vector>
#include <string>

namespace Dragon
{
    struct AppContext
    {
        bool is_running{ true };
        float time_since_start_sec; // NOTE: rename to time_sec
        float last_frame_dt_sec; // NOTE: rename to dt_sec
        float last_frame_dt_msec; // NOTE: rename to dt_msec
        float last_fps; // NOTE: rename to fps
    };
}
