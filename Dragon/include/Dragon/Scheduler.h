#pragma once

#include <functional>
#include <vector>

namespace Dragon
{
    class Scheduler
    {
    public:
        Scheduler();
        ~Scheduler() = default;
        Scheduler(const Scheduler&) = delete;
        Scheduler(Scheduler&&) noexcept = delete;
        Scheduler& operator=(const Scheduler&) = delete;
        Scheduler& operator=(Scheduler&&) noexcept = delete;
    public:
        void ScheduleAtNextFrameStart(std::function<void(void)> f) { m_at_next_frame_start.emplace_back(f); }
        void RunFrameStartCallbacks();
    private:
        std::vector<std::function<void(void)>> m_at_next_frame_start;
    };
}
