#pragma once

#include <unordered_map>


//Under Construction...

namespace Engin
{
	namespace HID
	{
		class GamepadInput
		{
		public:
			GamepadInput();
			~GamepadInput();

			void createGamepad();
			void removeGamepad(int GPindex);
			bool gamepadIsCreated(int GPindex);
			void update();

			int getAxisX(unsigned int buttonID, int GPindex);
			int getAxisY(unsigned int buttonID, int GPindex);
			bool buttonIsPressed(unsigned int buttonID, int GPindex);
			bool buttonWasPressed(unsigned int buttonID, int GPindex);
			bool buttonWasReleased(unsigned int buttonID, int GPindex);
			void getBallMotion(int GPindex, int ball, int &x, int &y); //int xy??
			//getHatPosition(int GPindex, int hat); Needs enum system or something

			int getNumGamepads();
			int getNumAxisControls(int GPindex);
			int getNumButtons(int GPindex);
			int getNumBalls(int GPindex);
			int getNumHats(int GPindex);

			void setAxisControlDeadZone(int value, int GPindex);

		private:
			std::unordered_map<unsigned int, bool> buttonMap;
			//int* axisControlDeadZone;
		};
	}
}