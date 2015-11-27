#include "Engin\Game\EventManager.h"

#include <SDL\SDL.h>

namespace Engin
{
	namespace Game
	{
		EventManager::EventManager(HID::KeyboardInput* kbInput, HID::MouseInput* mInput, HID::GamepadInput* gpInput) : quitState(false), keyboardInput(kbInput), mouseInput(mInput), gamepadInput(gpInput)
		{
		}
		EventManager::~EventManager()
		{
		}


		void EventManager::update()
		{
			processInput();
		}


		//Private:
		void EventManager::processInput()
		{
			static SDL_Event inputEvent;

			while (SDL_PollEvent(&inputEvent))
			{
				switch (inputEvent.type)
				{
					//Keyboard
				case SDL_KEYDOWN:
					keyboardInput->pressKey(inputEvent.key.keysym.sym);
					break;
				case SDL_KEYUP:
					keyboardInput->releaseKey(inputEvent.key.keysym.sym);
					break;

					//Mouse
				case SDL_MOUSEBUTTONDOWN:
					mouseInput->pressButton(inputEvent.button.button);
					break;
				case SDL_MOUSEBUTTONUP:
					mouseInput->releaseButton(inputEvent.button.button);
					break;
				case SDL_MOUSEMOTION:
					mouseInput->setMousePosition(inputEvent.motion.x, inputEvent.motion.y);
					break;
				case SDL_MOUSEWHEEL:
					mouseInput->moveMouseWheel(inputEvent.wheel.y);
					break;

					//Gamepad
					//*Joystick
				case SDL_JOYBUTTONDOWN:
					gamepadInput->pressButton(inputEvent.jbutton.button, inputEvent.jbutton.which);
					break;
				case SDL_JOYBUTTONUP:
					gamepadInput->releaseButton(inputEvent.jbutton.button, inputEvent.jbutton.which);
					break;
				case SDL_JOYAXISMOTION:
					gamepadInput->axisMotion(inputEvent.jaxis.axis, inputEvent.jaxis.value, inputEvent.jaxis.which);
					break;
				case SDL_JOYBALLMOTION:
					gamepadInput->ballMotion(inputEvent.jball.ball, inputEvent.jball.xrel, inputEvent.jball.yrel, inputEvent.jball.which);
					break;
				case SDL_JOYHATMOTION:
					gamepadInput->hatMotion(inputEvent.jhat.hat, inputEvent.jhat.value, inputEvent.jhat.which);
					break;
				case SDL_JOYDEVICEADDED:
					gamepadInput->addDevice(inputEvent.jdevice.which);
					break;
				case SDL_JOYDEVICEREMOVED:
					gamepadInput->removeDevice(inputEvent.jdevice.which);
					break;
					//*Controller (Is this necessary, or does SDL really handle these separately?)
				case SDL_CONTROLLERBUTTONDOWN:
					gamepadInput->pressButton(inputEvent.cbutton.button, inputEvent.cbutton.which);
					break;
				case SDL_CONTROLLERBUTTONUP:
					gamepadInput->pressButton(inputEvent.cbutton.button, inputEvent.cbutton.which);
					break;
				case SDL_CONTROLLERAXISMOTION:
					gamepadInput->axisMotion(inputEvent.caxis.axis, inputEvent.caxis.value, inputEvent.caxis.which);
					break;
				case SDL_CONTROLLERDEVICEADDED:
					gamepadInput->addDevice(inputEvent.cdevice.which);
					break;
				case SDL_CONTROLLERDEVICEREMOVED:
					gamepadInput->removeDevice(inputEvent.cdevice.which);
					break;

					//Touch Events
				case SDL_FINGERMOTION:
					break;
				case SDL_FINGERDOWN:
					break;
				case SDL_FINGERUP:
					break;

					//QUIT
				case SDL_QUIT:
					quitState = true;
					break;

					//WINDOW EVENTS
				case SDL_WINDOWEVENT:
					break;

				}
			}
		}
	}
}