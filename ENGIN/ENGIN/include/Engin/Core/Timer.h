#pragma once

#include <GL\glew.h>

namespace Engin
{
	namespace Core
	{
		class Timer
		{
		public:
			Timer();
			~Timer(){};

			// Returns the Global time in ms.
			static GLfloat getGlobalTime();

			// Returns the local time in ms.
			GLfloat getLocalTime();

			// Start the timer.
			GLfloat start();

			// Pause the timer and continue from pause.
			GLfloat pause();

			// Stop the timer.
			GLfloat stop();

			// Timer update.
			void update();

			// Returns whether the timer is started or not.
			bool isStarted();

			// Returns whether the timer is paused or not.
			bool isPaused();
		private:
			GLfloat startTime;
			GLfloat stopTime;
			GLfloat pauseTime;
			GLfloat localTime;

			bool started;
			bool paused;
		};
	}
}