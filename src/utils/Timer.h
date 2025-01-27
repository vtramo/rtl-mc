#pragma once

#include <chrono>

class Timer
{
public:
    void reset()
    {
        m_beg = Clock::now();
    }

    double elapsedInSeconds() const
    {
        return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
    }
private:
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double>;

    std::chrono::time_point<Clock> m_beg { Clock::now() };
};
