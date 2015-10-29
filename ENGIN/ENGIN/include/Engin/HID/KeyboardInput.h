#pragma once

#include <unordered_map>


//Call pressKey and releaseKey in EventHandler(or what ever it will be called) to update keyMap
//call update() in game loop to update previousKeyMap
//In the game's code you can call e.g. Engin::keyboardInput->keyIsDown(KEYBOARD_UP), which returns a true if up arrow is pressed.

namespace Engin
{
	namespace HID
	{
		enum KeyboardKey : unsigned int
		{
			//TODO
			//list all possible keys here ._.
		};

		class KeyboardInput
		{
			//friend class EventHandler

		public:
			KeyboardInput();
			~KeyboardInput();

			void update();

			bool keyIsPressed(KeyboardKey keyID);
			bool keyWasPressed(KeyboardKey keyID);
			bool keyWasReleased(KeyboardKey keyID);
			//bool keyComboWasPressed();
			//unsigned int* getAllPressedKeys(); ??

		private:
			void pressKey(unsigned int keyID);
			void releaseKey(unsigned int keyID);
			
			bool keyWasDown(unsigned int keyID);

			//store information of key states
			std::unordered_map<unsigned int, bool> keyMap;
			std::unordered_map<unsigned int, bool> previousKeyMap;
			//comboarray
			//
		};
	}
}