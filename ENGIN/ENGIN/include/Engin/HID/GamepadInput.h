#pragma once

#include <unordered_map>

//Still under construction . . .

//Buttons, Axes, Hats and Balls
//Call <<<<<TBD>>>>> in EventManager to update --
//call update() in game loop to update previousButtonMap
//In the game's code you can call e.g. gamepadInput->buttonIsPressed(HID::GAMEPAD_BUTTON_X), which returns a true if X button is pressed.

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
		enum GamepadButton : int
		{
			GAMEPAD_BUTTON_A,
			GAMEPAD_BUTTON_B,
			GAMEPAD_BUTTON_X,
			GAMEPAD_BUTTON_Y,
			GAMEPAD_BUTTON_BACK,
			GAMEPAD_BUTTON_GUIDE,
			GAMEPAD_BUTTON_START,
			GAMEPAD_BUTTON_LEFTSTICK,
			GAMEPAD_BUTTON_RIGHTSTICK,
			GAMEPAD_BUTTON_LEFTSHOULDER,
			GAMEPAD_BUTTON_RIGHTSHOULDER,
			GAMEPAD_BUTTON_DPAD_UP,
			GAMEPAD_BUTTON_DPAD_DOWN,
			GAMEPAD_BUTTON_DPAD_LEFT,
			GAMEPAD_BUTTON_DPAD_RIGHT,
			GAMEPAD_BUTTON_MAX
		};
		enum GamepadAxis : int
		{
			GAMEPAD_AXIS_LEFTX,
			GAMEPAD_AXIS_LEFTY,
			GAMEPAD_AXIS_RIGHTX,
			GAMEPAD_AXIS_RIGHTY,
			GAMEPAD_AXIS_TRIGGERLEFT,
			GAMEPAD_AXIS_TRIGGERRIGHT,
			GAMEPAD_AXIS_MAX
		};
		enum GamepadHatPosition
		{
			GAMEPAD_HAT_CENTERED = 0x00,
			GAMEPAD_HAT_UP = 0x01,
			GAMEPAD_HAT_RIGHT = 0x02,
			GAMEPAD_HAT_DOWN = 0x04,
			GAMEPAD_HAT_LEFT = 0x08,
			GAMEPAD_HAT_RIGHTUP = (GAMEPAD_HAT_RIGHT | GAMEPAD_HAT_UP),
			GAMEPAD_HAT_RIGHTDOWN = (GAMEPAD_HAT_RIGHT | GAMEPAD_HAT_DOWN),
			GAMEPAD_HAT_LEFTUP = (GAMEPAD_HAT_LEFT | GAMEPAD_HAT_UP),
			GAMEPAD_HAT_LEFTDOWN = (GAMEPAD_HAT_LEFT | GAMEPAD_HAT_DOWN),
		};

		class GamepadInput
		{
			friend class Game::EventManager;

		public:
			GamepadInput();
			~GamepadInput();

			void createGamepad(); 
			void removeGamepad(int GPindex);
			bool gamepadIsCreated(int GPindex);
			void update();

			int getAxisX(GamepadButton buttonID, int GPindex); //Axis thingys?
			int getAxisY(GamepadButton buttonID, int GPindex);
			bool buttonIsPressed(GamepadButton buttonID, int GPindex);
			bool buttonWasPressed(GamepadButton buttonID, int GPindex);
			bool buttonWasReleased(GamepadButton buttonID, int GPindex);
			int getBallXMotion(int GPindex, int ballIndex);
			int getBallYMotion(int GPindex, int ballIndex);
			GamepadHatPosition getHatPosition(int GPindex, int hatIndex);

			int getNumGamepads();
			int getNumAxisControls(int GPindex);
			int getNumButtons(int GPindex);
			int getNumBalls(int GPindex);
			int getNumHats(int GPindex);

			void setAxisControlDeadZone(int value, int GPindex);

		protected:
			void pressButton(unsigned int buttonID);
			void releaseButton(unsigned int buttonID);

		private:
			bool buttonWasDown(GamepadButton buttonID, int GPindex);

			std::unordered_map<unsigned int, bool> buttonMap;
			std::unordered_map<unsigned int, bool> previousButtonMap;
			std::unordered_map<unsigned int, short> axisDeadZoneMap;
		};
	}
}
