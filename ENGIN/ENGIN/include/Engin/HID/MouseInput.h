#pragma once

#include <glm\glm.hpp>

#include <unordered_map>


//Call setMousePosition, pressButton and releaseButton in EventManager to update mouse position coordinates and buttonMap
//call update() in game loop to update previousButtonMap
//In the game's code you can call e.g. Engin::mouseInput->buttonIsPressed(HID::MOUSEBUTTON_LEFT), which returns a true if left mouse button is pressed.


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
			MOUSEBUTTON_X1 = 4,
			MOUSEBUTTON_X2 = 5
		};

		class MouseInput
		{
			friend class Game::EventManager;

		public:
			MouseInput();
			~MouseInput();

			void update();

			bool ButtonIsPressed(MouseButton buttonID);
			bool ButtonWasPressed(MouseButton buttonID);
			bool ButtonWasReleased(MouseButton buttonID);
			//TODO: Mouse wheel stuff
			//bool mouseWheelMoved(); ??

			//Getters
			void getMousePosition(int &xPos, int &yPos) { xPos = mousePositionX; yPos = mousePositionY; }
			//void getMouseWheelPosition(int &position); ??

		protected:
			void pressButton(unsigned int buttonID);
			void releaseButton(unsigned int buttonID);
			void setMousePosition(int x, int y);

		private:
			bool buttonWasDown(unsigned int buttonID);

			std::unordered_map<unsigned int, bool> buttonMap;
			std::unordered_map<unsigned int, bool> previousButtonMap;
			int mousePositionX;
			int mousePositionY;
			//TODO: mouse moved event system
		};
	}
}