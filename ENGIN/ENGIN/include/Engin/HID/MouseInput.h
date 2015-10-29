#pragma once

#include <glm\glm.hpp>

#include <unordered_map>


//Call setMousePosition, pressButton and releaseButton in EventHandler(or what ever it will be called) to update mouse position coordinates and buttonMap
//call update() in game loop to update previousButtonMap
//In the game's code you can call e.g. Engin::keyboardInput->keyIsDown(KEYBOARD_UP), which returns a true if up arrow is pressed.


namespace Engin
{
	namespace HID
	{
		enum MouseButton : unsigned int
		{
			//TODO
			//list all possible mouse buttons...
		};

		class MouseInput
		{
			//friend class EventHandler

		public:
			MouseInput();
			~MouseInput();

			void update();

			bool ButtonIsPressed(MouseButton buttonID);
			bool ButtonWasPressed(MouseButton buttonID);
			bool ButtonWasReleased(MouseButton buttonID);
			//bool mouseWheelMoved(); ??

			//Getters
			//NOTE: if we don't need both of these mouseposition getters, the useless one can be removed
			void getMousePosition(float &xPos, float &yPos) { xPos = mousePosition.x; yPos = mousePosition.y; }
			glm::vec2 getMousePosition() { return mousePosition; }
			//void getMouseWheelPosition(int &position); ??

		private:
			void pressButton(unsigned int buttonID);
			void releaseButton(unsigned int buttonID);
			void setMousePosition(float x, float y);

			bool buttonWasDown(unsigned int buttonID);

			std::unordered_map<unsigned int, bool> buttonMap;
			std::unordered_map<unsigned int, bool> previousButtonMap;
			glm::vec2 mousePosition;
		};
	}
}