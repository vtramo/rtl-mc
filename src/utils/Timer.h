#pragma once

#include <chrono>

/*!
 *  \brief A simple timer class for measuring elapsed time in seconds.
 *
 * The `Timer` class provides functionality to start a timer and measure the elapsed time
 * since the timer was started or last reset. It uses the `std::chrono` library for
 * high-resolution time measurements.
 *
 * Example usage:
 * \code
 * Timer timer {};
 * // Perform some operations...
 * double elapsed { timer.elapsedInSeconds() }; // Get elapsed time in seconds
 * \endcode
 */
class Timer
{
public:
    /*!
     *  \brief Resets the timer to the current time.
     *
     * This function sets the internal start time to the current time, effectively
     * restarting the timer.
     */
    void reset()
    {
        m_beg = Clock::now();
    }

    /*!
     *  \brief Returns the elapsed time in seconds since the timer was started or last reset.
     *
     * This function calculates the difference between the current time and the start time
     * (or the time of the last reset) and returns the result in seconds as a `double`.
     *
     * \return The elapsed time in seconds.
     */
    double elapsedInSeconds() const
    {
        return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
    }
private:
    using Clock = std::chrono::steady_clock; //!< Clock type used for time measurements.
    using Second = std::chrono::duration<double>; //!< Duration type representing seconds.

    std::chrono::time_point<Clock> m_beg { Clock::now() }; //!< The start time of the timer.
};
