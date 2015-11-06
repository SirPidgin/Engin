#pragma once

#include <vector>

#include <glm\vec2.hpp>

#include "Component.h"


namespace Engin
{
	namespace Game
	{
		class GameObject
		{
		public:
			GameObject();
			~GameObject();

			void update();
			void draw();

			//add or enable(?)
			void addComponent(Component* newComponent);

			//Pure Virtual Getters & Setters
			//Transform Component
			virtual glm::vec2 getPosition() = 0;
			virtual float getXPosition() = 0;
			virtual float getYPosition() = 0;
			virtual float getScale() = 0;
			virtual float getRotation() = 0;
			
			virtual void setPosition() = 0;
			virtual void setScale() = 0;
			virtual void setRotation() = 0;

			//SpriteComponent
			virtual void setCurrentSprite() = 0;
			
		private:
			std::vector<Component*> components;
		};
	}
}