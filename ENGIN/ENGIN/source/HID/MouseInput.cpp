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
			//Reset mouse wheel position
			mouseWheelYPosition = 0;
			//Update prevMousePosition
			prevMousePosX = mousePositionX;
			prevMousePosY = mousePositionY;
		}


		bool MouseInput::buttonIsPressed(MouseButton buttonID)
		{
			auto it = buttonMap.find(buttonID);
			if (it != buttonMap.end())
			{
				return it->second;
			}
			return false;
		}

		
		bool MouseInput::buttonWasPressed(MouseButton buttonID)
		{
			if (buttonIsPressed(buttonID) == true && buttonWasDown(buttonID) == false)
			{
				return true;
			}
			return false;
		}


		bool MouseInput::buttonWasReleased(MouseButton buttonID)
		{
			if (buttonIsPressed(buttonID) == false && buttonWasDown(buttonID) == true)
			{
				return true;
			}
			return false;
		}


		bool MouseInput::mouseWheelWasMoved(MouseWheel direction)
		{
			if (direction < 0)
			{
				if (mouseWheelYPosition < 0)
				{
					return true;
				}
				return false;
			}
			else
			{
				if (mouseWheelYPosition > 0)
				{
					return true;
				}
				return false;
			}
		}


		bool MouseInput::mouseWasMoved()
		{
			if (mousePositionX != prevMousePosX || mousePositionY != prevMousePosY)
			{
				if (prevMousePosX != -1 && prevMousePosY != 1)
				{
					return true;
				}
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


		void MouseInput::moveMouseWheel(int y)
		{
			mouseWheelYPosition += y;
		}
	}
}