/*
 * Timer.h
 *
 *  Created on: 11.07.2017
 *      Author: stefan
 */

#ifndef CORE_TIMER_H_
#define CORE_TIMER_H_

#include <SDL2/SDL.h>
#include <functional>

namespace core
{

	class Timer
	{
	public:
		Timer(const uint32_t time, const uint32_t repeat);
		virtual ~Timer();

		void setCallback(std::function<void(uint32_t)> f)
		{
			m_callback = f;
		}
		void reset(const uint32_t time, const uint32_t repeat);

		void start();
		void stop();
		void pause();
		void update();

	private:
		uint32_t m_startTime;
		uint32_t m_time;
		uint32_t m_repeat;
		uint32_t m_execs;
		bool m_paused;
		std::function<void(uint32_t)> m_callback = nullptr;
	};

} /* namespace core */

#endif /* CORE_TIMER_H_ */
