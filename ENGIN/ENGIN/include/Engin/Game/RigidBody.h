#pragma once

#include <glm\vec2.hpp>

#include "Component.h"


namespace Engin
{
	namespace Game
	{
		class RigidBody : public Component
		{
		public:
			RigidBody();
			~RigidBody();

		private:
			glm::vec2 velocity;
			glm::vec2 acceleration;
			float mass;
		};
	}
}