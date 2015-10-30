#include "Engin\Core\Timer.h"
#include <SDL\SDL_timer.h>

namespace Engin
{
	namespace Core
	{
		Timer::Timer()
		{
			startTime = 0.0f;
			stopTime = 0.0f;
			pauseTime = 0.0f;
			globalTime = 0.0f;
			localTime = 0.0f;

			started = false;
			paused = false;

			globalTime = SDL_GetTicks();
		}

		GLfloat Timer::getGlobalTime()
		{
			return globalTime;
		}

		GLfloat Timer::getLocalTime()
		{	
			localTime = 0.0f;

			if (started)
			{
				if (paused)
				{
					localTime = pauseTime;
				}
				else
				{
					localTime = globalTime - startTime;
				}
			}
			return localTime;
		}

		GLfloat Timer::start()
		{	
			if (started && paused) //To unpause.
			{
				paused = false;
				startTime = globalTime - pauseTime;
				pauseTime = 0.0f;
			}
			else
			{
				started = true;
				paused = false;

				startTime = globalTime;
				pauseTime = 0.0f;
				return startTime;
			}
		}

		GLfloat Timer::stop()
		{
			started = false;
			paused = false;

			stopTime = globalTime;

			startTime = 0.0f;
			pauseTime = 0.0f;
			return stopTime;
		}

		GLfloat Timer::pause()
		{
			if (started == true && paused == false)
			{
				paused = true;

				pauseTime = globalTime - startTime;
				return pauseTime;
			}
			else
			{
				return pauseTime;
			}
		}

		void Timer::update()
		{
			globalTime = SDL_GetTicks();
		}

		bool Timer::isStarted()
		{
			return started;
		}

		bool Timer::isPaused()
		{
			return paused && started;
		}
	}
}