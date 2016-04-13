#include "PTPhysicsWorld.h"
#include "glm\gtc\matrix_transform.hpp"

PTPhysicsWorld::PTPhysicsWorld(GLfloat scale) : scale(scale)
{
	
}

PTPhysicsWorld::~PTPhysicsWorld()
{

}

void PTPhysicsWorld::update(GLfloat step)
{
	for (int i = 0; i < rigidBodies.size(); i++)
	{
		rigidBodies[i]->setCollisionOff();
	}

	for (int i = 0; i < rigidBodies.size(); i++)
	{
		for (int j = 0; j < rigidBodies.size(); j++)
		{
			if (i != j)
			{
				if (isCollidingAABB(rigidBodies[i], rigidBodies[j]))
				{
					rigidBodies[i]->setCollisionOn();
					rigidBodies[j]->setCollisionOn();
				}
			}	
		}		
	}

	for (int i = 0; i < rigidBodies.size(); i++)
	{
		rigidBodies[i]->update(step);
	}
}

void PTPhysicsWorld::addRigidBody(PTRigidBody* body, glm::vec2& position)
{
	body->setPosition(position);
	rigidBodies.push_back(body);
}

void PTPhysicsWorld::setGravity(glm::vec2 gravity)
{
	this->gravity = gravity*scale;
}

glm::vec2 PTPhysicsWorld::getGravity()
{
	return gravity;
}

std::vector<PTRigidBody*> PTPhysicsWorld::getBodies()
{
	return rigidBodies;
}

bool PTPhysicsWorld::isCollidingAABB(PTRigidBody* body1, PTRigidBody* body2)
{
	if (glm::abs(body1->getPosition().x - body2->getPosition().x) > (body1->getHalfSize().x + body2->getHalfSize().x))
	{
		return false;
	}
	if (glm::abs(body1->getPosition().y - body2->getPosition().y) > (body1->getHalfSize().y + body2->getHalfSize().y))
	{
		return false;
	}
	return true;
}

bool PTPhysicsWorld::isCollidingSAT(PTRigidBody* body1, PTRigidBody* body2)
{
	float body1Min = 0.0f;
	float body1Max = 0.0f;
	float body2Min = 0.0f;
	float body2Max = 0.0f;

	glm::vec2 normal;
	glm::vec2 slope;

	float angle1 = body1->getRotation();
	float angle2 = body2->getRotation();

	for (int i = 0; i < 8; i++)
	{
		body1Min = 0.0f;
		body1Max = 0.0f;
		body2Min = 0.0f;
		body2Max = 0.0f;

		

	}

	return true;
}