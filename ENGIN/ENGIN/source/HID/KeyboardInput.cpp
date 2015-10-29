#include "Engin\HID\KeyboardInput.h"

namespace Engin
{
	namespace HID
	{
		KeyboardInput::KeyboardInput()
		{
		}
		KeyboardInput::~KeyboardInput()
		{
		}


		void KeyboardInput::update()
		{
			//update previousKeyMap
			for (auto& it : keyMap)
			{
				previousKeyMap.at(it.first) = it.second;
			}
		}


		bool KeyboardInput::keyIsPressed(KeyboardKey keyID)
		{
			auto it = keyMap.find(keyID);
			if (it != keyMap.end())
			{
				return it->second;
			}
			return false;
		}


		bool KeyboardInput::keyWasPressed(KeyboardKey keyID)
		{
			if (keyIsPressed(keyID) == true && keyWasDown(keyID) == false)
			{
				return true;
			}
			return false;
		}


		bool KeyboardInput::keyWasReleased(KeyboardKey keyID)
		{
			if (keyIsPressed(keyID) == false && keyWasDown(keyID) == true)
			{
				return true;
			}
			return false;
		}


		//Private:
		void KeyboardInput::pressKey(unsigned int keyID)
		{
			keyMap.at(keyID) = true;
		}


		void KeyboardInput::releaseKey(unsigned int keyID)
		{
			keyMap.at(keyID) = false;
		}


		bool KeyboardInput::keyWasDown(unsigned int keyID)
		{
			auto it = previousKeyMap.find(keyID);
			if (it != previousKeyMap.end())
				return it->second;
			else
				return false;
		}
	}
}