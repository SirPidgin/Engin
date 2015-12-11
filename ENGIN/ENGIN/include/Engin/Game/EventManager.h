#pragma once

#include "Engin\HID\KeyboardInput.h"
#include "Engin\HID\MouseInput.h"
#include "Engin\HID\GamepadInput.h"

/*
Class for managing events.
Update is handled in engine.cpp.
*/

namespace Engin
{
	namespace Game
	{
		class EventManager
		{
		public:
			EventManager(HID::KeyboardInput* kbInput, HID::MouseInput* mInput, HID::GamepadInput* gbInput);
			~EventManager();

			void update();

			//Getters
			bool userQuit() { return quitState; }

		private:
			void processInput();

			HID::KeyboardInput* keyboardInput;
			HID::MouseInput* mouseInput;
			HID::GamepadInput* gamepadInput;

			bool quitState;
		};
	}
}