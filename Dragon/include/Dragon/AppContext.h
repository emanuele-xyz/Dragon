#pragma once

#include <vector>
#include <string>

namespace Dragon
{
    // TODO: maybe split this into AppContext and GfxSettings
    struct AppContext
    {
        bool is_running{ true };
        bool vsync{ false };
        size_t msaa_index;
        std::vector<std::string> msaa_settings;
        float time_since_start_sec;
        float last_frame_dt_sec;
        float last_frame_dt_msec;
        float last_fps;
    };
}
