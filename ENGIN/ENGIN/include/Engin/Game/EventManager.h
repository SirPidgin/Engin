#pragma once

#include "Engin\HID\KeyboardInput.h"
#include "Engin\HID\MouseInput.h"
#include "Engin\HID\GamepadInput.h"


namespace Engin
{
	namespace Game
	{
		class EventManager
		{
		public:
			EventManager(HID::KeyboardInput* kbInput, HID::MouseInput* mInput);
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