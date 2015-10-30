#include "Engin\HID\MouseInput.h"

namespace Engin
{
	namespace HID
	{
		MouseInput::MouseInput()
		{
		}
		MouseInput::~MouseInput()
		{
		}


		void MouseInput::update()
		{
			//update previousButtonMap
			for (auto& it : buttonMap)
			{
				previousButtonMap[it.first] = it.second;
			}
		}


		bool MouseInput::ButtonIsPressed(MouseButton buttonID)
		{
			auto it = buttonMap.find(buttonID);
			if (it != buttonMap.end())
			{
				return it->second;
			}
			return false;
		}

		
		bool MouseInput::ButtonWasPressed(MouseButton buttonID)
		{
			if (ButtonIsPressed(buttonID) == true && buttonWasDown(buttonID) == false)
			{
				return true;
			}
			return false;
		}


		bool MouseInput::ButtonWasReleased(MouseButton buttonID)
		{
			if (ButtonIsPressed(buttonID) == false && buttonWasDown(buttonID) == true)
			{
				return true;
			}
			return false;
		}


		//Private:
		void MouseInput::pressButton(unsigned int buttonID)
		{
			buttonMap[buttonID] = true;
		}


		void MouseInput::releaseButton(unsigned int buttonID)
		{
			buttonMap[buttonID] = false;
		}


		void MouseInput::setMousePosition(int x, int y)
		{
			mousePositionX = x;
			mousePositionX = y;
		}


		bool MouseInput::buttonWasDown(unsigned int buttonID)
		{
			auto it = previousButtonMap.find(buttonID);
			if (it != previousButtonMap.end())
				return it->second;
			else
				return false;
		}
	}
}