#pragma once

#include <chrono>

namespace vi
{
	using Clock = std::chrono::high_resolution_clock;

	class Timer
	{
	public:
		Timer() { Restart(); }

		void Restart()
		{
			m_start = m_clock.now();
		}

		float Elapsed()
		{
			constexpr float us_to_secs = 1.0f / 1000000.0f;
			return std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now() - m_start).count() * us_to_secs;
		}

	private:
		Clock m_clock;
		Clock::time_point m_start;
	};
}