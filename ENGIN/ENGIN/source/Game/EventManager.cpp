#include "Engin\Game\EventManager.h"

#include <SDL\SDL.h>

namespace Engin
{
	namespace Game
	{
		EventManager::EventManager(HID::KeyboardInput* kbInput, HID::MouseInput* mInput) : quitState(false), keyboardInput(kbInput), mouseInput(mInput)
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
					//HID
				case SDL_KEYDOWN:
					keyboardInput->pressKey(inputEvent.key.keysym.sym);
					break;
				case SDL_KEYUP:
					keyboardInput->releaseKey(inputEvent.key.keysym.sym);
					break;

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

				case SDL_JOYBUTTONDOWN:
					break;
				case SDL_JOYBUTTONUP:
					break;
				case SDL_JOYAXISMOTION:
					break;
				case SDL_JOYBALLMOTION:
					break;
				case SDL_JOYHATMOTION:
					break;
				case SDL_JOYDEVICEADDED:
					break;
				case SDL_JOYDEVICEREMOVED:
					break;



					//QUIT
				case SDL_QUIT:
					quitState = true;
					break;

					//WINDOW EVENTS


				}
			}
		}
	}
}