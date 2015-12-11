#pragma once

#include <glm\vec2.hpp>

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

/*
Rigid body (physics) component for game objects.
Not ready for use since there are currently no physics in the engine.
*/

namespace Engin
{
	namespace Game
	{
		//enum ForceMode
		//{
		//	Force,
		//	Acceleration,
		//	Impulse,
		//	VelocityChange
		//};

		class RigidBody : public Component
		{
		public:
			RigidBody(GameObject* o);
			~RigidBody();

			void update();

			void addForce(glm::vec2 force);
			void addRelativeForce(glm::vec2 force);
			void addForceAtPosition(glm::vec2 force, glm::vec2 position);
			void addTorque(glm::vec2 torque);
			void addRelativeTorque(glm::vec2 torque);

			bool isColliding(GameObject* other);
			
			//Setters
			void setMass(float newMass);
			void setCollisionMode(CollisionMode newCollisionMode);
			void setCollisionRadius(float newRadius);

			//Getters
			glm::vec2 getClosestCollidingPoint(glm::vec2 point);
			glm::vec2 getVelocity();
			glm::vec2 getRelativeVelocity(glm::vec2 relativePoint);
			float getMass();
			float getCollisionCircleRadius();

		private:
			glm::vec2 position;
			float rotation;

			glm::vec2 velocity;
			glm::vec2 angularVelocity;
			glm::vec2 maxAngularVelocity;
			glm::vec2 acceleration;
			glm::vec2 centerOfMass;
			glm::vec2 worldCenterOfMass;

			float drag;
			float angularDrag;
			float mass;
			float collisionRadius;

			CollisionMode collision;
			bool freezeRotation;
			bool useGravity;
		};
	}
}