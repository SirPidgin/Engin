#pragma once

#include <unordered_map>

namespace Engin
{
	namespace HID
	{
		class KeyboardInput
		{
		public:
			KeyboardInput();
			~KeyboardInput();

			void update();

			bool keyIsDown(unsigned int keyID);
			bool keyWasPressed(unsigned int keyID);
			bool keyWasReleased(unsigned int keyID);
			//bool keyComboWasPressed();

		private:
			std::unordered_map<unsigned int, bool> keyMap;
			std::unordered_map<unsigned int, bool> previousKeyMap;
			//comboarray
		};
	}
}