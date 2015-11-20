#pragma once

#include <glm\vec2.hpp>

#include "Component.h"


namespace Engin
{
	namespace Game
	{
		class Transform : public Component
		{
		public:
			Transform(GameObject* o) : Component(o) {}
			~Transform(){}

			//Getters
			glm::vec2 getPosition() { return position; }
			float getXPosition() { return position.x; }
			float getYPosition() { return position.y; }
			float getScale() { return scale; }
			float getRotation() { return rotation; }

			//Setters
			void setPosition(glm::vec2 newPos) { position = newPos; }
			void setXPosition(float xPos) { position.x = xPos; }
			void setYPosition(float yPos) { position.y = yPos; }
			void setRotation(float newRot) { rotation = newRot; }
			void setScale(float newScale) { scale = newScale; }

		private:
			glm::vec2 position = glm::vec2(0.0f, 0.0f);
			float rotation = 0.0f;
			float scale = 1.0f;
		};
	}
}