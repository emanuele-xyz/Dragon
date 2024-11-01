#include <Dragon/pch.h>
#include <Dragon/Scheduler.h>

namespace Dragon
{
    Scheduler::Scheduler()
        : m_at_next_frame_start{}
    {
    }

    void Scheduler::RunFrameStartCallbacks()
    {
        std::for_each(m_at_next_frame_start.cbegin(), m_at_next_frame_start.cend(), [](const auto& f) { f(); });
        m_at_next_frame_start.clear();
    }
}
