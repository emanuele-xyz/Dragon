#pragma once

#include <vector>
#include <string>

namespace Dragon
{
    struct AppContext
    {
        bool is_running{ true };
        float time_since_start_sec{}; // TODO: rename to time_sec
        float last_frame_dt_sec{}; // TODO: rename to dt_sec
        float last_frame_dt_msec{}; // TODO: rename to dt_msec
        float last_fps{}; // TODO: rename to fps
        float assets_load_time_sec{};
    };
}
