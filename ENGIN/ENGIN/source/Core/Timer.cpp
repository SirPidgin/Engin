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
			localTime = 0.0f;

			started = false;
			paused = false;
		}

		GLfloat Timer::getGlobalTime()
		{
			return SDL_GetTicks();
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
					localTime = getGlobalTime() - startTime;
				}
			}
			return localTime;
		}

		GLfloat Timer::start()
		{			
			started = true;
			paused = false;

			startTime = getGlobalTime();
			pauseTime = 0.0f;
			return startTime;	
		}

		GLfloat Timer::stop()
		{
			started = false;
			paused = false;

			stopTime = getGlobalTime();

			startTime = 0.0f;
			pauseTime = 0.0f;
			return stopTime;
		}

		GLfloat Timer::pause()
		{
			if (started && paused) // Unpause.
			{
				paused = false;
				startTime = getGlobalTime() - pauseTime;
				pauseTime = 0.0f;
				return startTime;
			}
			else if (started == true && paused == false) // Pause
			{
				paused = true;

				pauseTime = getGlobalTime() - startTime;
				return pauseTime;
			}
			else
			{
				return pauseTime;
			}
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