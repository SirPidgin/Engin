#include "Engin\Game\RigidBody.h"


namespace Engin
{
	namespace Game
	{
		RigidBody::RigidBody(GameObject* o) : Component(o)
		{
		}
		RigidBody::~RigidBody()
		{
		}


		void RigidBody::update()
		{
			position = ownerObject->accessComponent<Transform>()->getPosition();
			rotation = ownerObject->accessComponent<Transform>()->getRotation();
		}


		void RigidBody::addForce(glm::vec2 force)
		{
			//
		}


		void RigidBody::addRelativeForce(glm::vec2 force)
		{
			//
		}


		void RigidBody::addForceAtPosition(glm::vec2 force, glm::vec2 position)
		{
			//
		}


		void RigidBody::addTorque(glm::vec2 torque)
		{
			//
		}


		void RigidBody::addRelativeTorque(glm::vec2 torque)
		{
			//
		}


		bool RigidBody::isColliding(GameObject* other)
		{
			float distance = glm::length(glm::vec2(position - other->accessComponent<Transform>()->getPosition()));
			if (distance <= this->collisionRadius + other->accessComponent<RigidBody>()->getCollisionCircleRadius())
			{
				return true;
			}
			return false;
		}


		void RigidBody::setMass(float newMass)
		{
			mass = newMass;
		}


		void RigidBody::setCollisionMode(CollisionMode newCollisionMode)
		{
			collision = newCollisionMode;
		}


		void RigidBody::setCollisionRadius(float newRadius)
		{
			collisionRadius = newRadius;
		}


		glm::vec2 RigidBody::getClosestCollidingPoint(glm::vec2 point)
		{
			return glm::vec2(0.0f);
		}


		glm::vec2 RigidBody::getVelocity()
		{
			return glm::vec2(0.0f);
		}


		glm::vec2 RigidBody::getRelativeVelocity(glm::vec2 relativePoint)
		{
			return glm::vec2(0.0f);
		}


		float RigidBody::getMass()
		{
			return mass;
		}


		float RigidBody::getCollisionCircleRadius()
		{
			return collisionRadius;
		}
	}
}