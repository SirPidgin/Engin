#pragma once

#include "GameObject.h"


namespace Engin
{
	namespace Game
	{
		class Component : public GameObject
		{
		public:
			Component();
			virtual ~Component();

			virtual void draw() = 0;
		};
	}
}