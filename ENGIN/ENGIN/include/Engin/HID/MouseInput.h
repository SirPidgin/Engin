#pragma once

#include <unordered_map>


namespace Engin
{
	namespace HID
	{
		class MouseInput
		{
		public:
			MouseInput();
			~MouseInput();

			bool mouseButtonIsPressed(unsigned int buttonID);
			bool mouseButtonWasPressed(unsigned int buttonID);
			bool mouseButtonWasReleased(unsigned int buttonID);
			//bool mouseWheelMoved();

			//void getMouseWheelPosition(int &position);
			void getMousePosition(float &xPos, float &yPos);

		private:
			std::unordered_map<unsigned int, bool> buttonMap;
		};
	}
}