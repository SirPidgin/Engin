#pragma once

#include <glm\vec2.hpp>

#include "Engin\Resources\Texture.h"

/*
Template for creating new components:
	class newComponent : public Component
	{
		//
	};

Components can be accessed from gameObject by calling accessComponent<>:
myGameObject.accessComponent<myComponent>->componentMehtod();
All components need to be added to the game object with addComponent<>.
*/

namespace Engin
{
	namespace Game
	{
		//Collision mode used in Rigid Body component
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