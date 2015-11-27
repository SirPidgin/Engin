#pragma once

#include <glm\vec2.hpp>

#include "Engin\Resources\Texture.h"


namespace Engin
{
	namespace Game
	{
		enum CollisionMode
		{
			CM_NoCollision,
			CM_Circle,
		};

		class GameObject;
		class Component
		{
		public:
			Component();
			Component(GameObject* o);
			~Component();

			virtual void draw();
			virtual void update();

		protected:
			GameObject* ownerObject;
		};
	}
}