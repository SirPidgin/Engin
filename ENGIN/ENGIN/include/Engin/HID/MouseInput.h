#pragma once

#include <glm\vec2.hpp>

#include <unordered_map>


//Call setMousePosition, pressButton and releaseButton in EventManager to update mouse position coordinates and buttonMap
//call update() in game loop to update previousButtonMap
//In the game's code you can call e.g. mouseInput->buttonIsPressed(HID::MOUSEBUTTON_LEFT), which returns a true if left mouse button is pressed.


//Forward Declaration
namespace Engin
{
	namespace Game
	{
		class EventManager;
	}
}

namespace Engin
{
	namespace HID
	{
		enum MouseButton : unsigned int
		{
			MOUSEBUTTON_LEFT = 1,
			MOUSEBUTTON_MIDDLE = 2,
			MOUSEBUTTON_RIGHT = 3,
			MOUSEBUTTON_X1 = 4, //Sidebuttons?
			MOUSEBUTTON_X2 = 5,
			//Are there more mouse buttons?
		};
		enum MouseWheel : int
		{
			MOUSEWHEEL_UP = 1,
			MOUSEWHEEL_DOWN = -1,
		};

		class MouseInput
		{
			friend class Game::EventManager;

		public:
			MouseInput();
			~MouseInput();

			void update();

			bool buttonIsPressed(MouseButton buttonID);
			bool buttonWasPressed(MouseButton buttonID);
			bool buttonWasReleased(MouseButton buttonID);
			bool mouseWheelWasMoved(MouseWheel direction);
			bool mouseWasMoved();

			//Getters
			glm::ivec2 getMousePosition() { return mousePosition; }
			int getMouseXPosition() { return mousePosition.x; }
			int getMouseYPosition() { return mousePosition.y; }

		protected:
			void pressButton(unsigned int buttonID);
			void releaseButton(unsigned int buttonID);
			void setMousePosition(int x, int y);
			void moveMouseWheel(int y);

		private:
			bool buttonWasDown(unsigned int buttonID);

			std::unordered_map<unsigned int, bool> buttonMap;
			std::unordered_map<unsigned int, bool> previousButtonMap;
			glm::ivec2 mousePosition;
			glm::ivec2 prevMousePos = glm::ivec2(-1, -1);
			int mouseWheelYPosition = 0;
		};
	}
}