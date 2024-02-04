/*
 * Timer.cpp
 *
 *  Created on: 11.07.2017
 *      Author: stefan
 */

#include "engine/core/Timer.h"

namespace core
{

    Timer::Timer(const uint32_t time, const int32_t repeat) :
        m_startTime(0), m_time(time), m_repeat(repeat), m_execs(0), m_paused(false)
    {
    }

    Timer::~Timer()
    {
        // TODO Auto-generated destructor stub
    }

    void Timer::start()
    {
        m_startTime = SDL_GetTicks();
        m_execs = 0;
    }
    void Timer::stop() { m_startTime = 0; }
    void Timer::pause() { m_paused = true; }

    void Timer::update()
    {
        if (m_startTime == 0)
            return;

        Uint32 currentTime = SDL_GetTicks();
        if (m_startTime + m_time <= currentTime)
        {
            m_callback(m_execs);

            m_startTime = currentTime;
            if (m_repeat > 0)
            {
                m_execs++;
                if (m_execs > static_cast<uint32_t>(m_repeat))
                {
                    stop();
                }
            }
        }
    }

} /* namespace core */
