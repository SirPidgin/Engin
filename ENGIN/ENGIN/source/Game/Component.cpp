#include <string>

#include "Engin\Game\Component.h"
#include "Engin\Game\RigidBody.h"


namespace Engin
{
	namespace Game
	{
		Component::Component() : ownerObject(nullptr)
		{}
		Component::Component(GameObject* o) : ownerObject(o)
		{}
		Component::~Component()
		{}

		void Component::draw()
		{}
		void Component::update()
		{}

		//Transform
		glm::vec2 Component::getPosition()
		{
			return glm::vec2(0.0f);
		}
		float Component::getXPosition()
		{
			return NULL;
		}
		float Component::getYPosition()
		{
			return NULL;
		}
		float Component::getScale()
		{
			return NULL;
		}
		float Component::getRotation()
		{
			return NULL;
		}
		void Component::setPosition(glm::vec2 newPos)
		{}
		void Component::setXPosition(float xPos)
		{}
		void Component::setYPosition(float yPos)
		{}
		void Component::setScale(float newScale)
		{}
		void Component::setRotation(float newRot)
		{}

		//Sprite
		void Component::setCurrentSprite(Resources::Texture* texture)
		{}

		//RigidBody
		void Component::addForce(glm::vec2 force)
		{}
		void Component::addRelativeForce(glm::vec2 force)
		{}
		void Component::addForceAtPosition(glm::vec2 force, glm::vec2 position)
		{}
		void Component::addTorque(glm::vec2 torque)
		{}
		void Component::addRelativeTorque(glm::vec2 torque)
		{}
		bool Component::isColliding(GameObject* other)
		{
			return NULL;
		}
		void Component::setMass(float newMass)
		{}
		void Component::setCollisionMode(CollisionMode newCollisionMode)
		{}
		void Component::setCollisionRadius(float newRadius)
		{}
		glm::vec2 Component::getClosestCollidingPoint(glm::vec2 point)
		{
			return glm::vec2(0.0f);
		}
		glm::vec2 Component::getVelocity()
		{
			return glm::vec2(0.0f);
		}
		glm::vec2 Component::getRelativeVelocity(glm::vec2 relativePoint)
		{
			return glm::vec2(0.0f);
		}
		float Component::getCollisionCircleRadius()
		{
			return NULL;
		}

		//Custom Components
		void Component::setIntVariable(std::string variableName, int newValue)
		{}
		int Component::getIntVariable(std::string variableName)
		{
			return NULL;
		}
		void Component::setFloatVariable(std::string variableName, float newValue)
		{}
		float Component::getFloatVariable(std::string variableName)
		{
			return NULL;
		}
		void Component::setVec2Variable(std::string variableName, glm::vec2 newValue)
		{}
		glm::vec2 Component::getVec2Variable(std::string variableName)
		{
			return glm::vec2(0.0f);
		}
	}
}