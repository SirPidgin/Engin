#include "PTPhysicsWorld.h"

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

bool PTPhysicsWorld::isColliding()
{


	return 0;
}