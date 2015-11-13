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

			//Pure Virtual Getters & Setters
			//Transform
			virtual glm::vec2 getPosition();
			virtual float getXPosition();
			virtual float getYPosition();
			virtual float getScale();
			virtual float getRotation();

			virtual void setPosition(glm::vec2 newPos);
			virtual void setXPosition(float xPos);
			virtual void setYPosition(float yPos);
			virtual void setScale(float newScale);
			virtual void setRotation(float newRot);

			//Sprite
			virtual void setCurrentSprite(Resources::Texture* texture);

			//RigidBody
			virtual void addForce(glm::vec2 force);
			virtual void addRelativeForce(glm::vec2 force);
			virtual void addForceAtPosition(glm::vec2 force, glm::vec2 position);
			virtual void addTorque(glm::vec2 torque);
			virtual void addRelativeTorque(glm::vec2 torque);
			virtual bool isColliding(GameObject* other);
			virtual void setMass(float newMass);
			virtual void setCollisionMode(CollisionMode newCollisionMode);
			virtual void setCollisionRadius(float newRadius);
			virtual glm::vec2 getClosestCollidingPoint(glm::vec2 point);
			virtual glm::vec2 getVelocity();
			virtual glm::vec2 getRelativeVelocity(glm::vec2 relativePoint);
			virtual float getCollisionCircleRadius();

			//Custom Components
			virtual void setIntVariable(std::string variableName, int newValue);
			virtual int getIntVariable(std::string variableName);

			virtual void setFloatVariable(std::string variableName, float newValue);
			virtual float getFloatVariable(std::string variableName);

			virtual void setVec2Variable(std::string variableName, glm::vec2 newValue);
			virtual glm::vec2 getVec2Variable(std::string variableName);

		protected:
			GameObject* ownerObject;
		};
	}
}