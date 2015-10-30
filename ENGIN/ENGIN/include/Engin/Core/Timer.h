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
			static GLfloat getGlobalTime();
			GLfloat getLocalTime();
			GLfloat start();
			GLfloat pause();
			GLfloat stop();
			void update();

			bool isStarted();
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