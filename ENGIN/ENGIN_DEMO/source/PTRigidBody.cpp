#include "PTRigidBody.h"

PTRigidBody::PTRigidBody(PTPhysicsWorld* world) : world(world)
{

}

PTRigidBody::~PTRigidBody()
{

}

void PTRigidBody::update(GLfloat step)
{
	velocityOld = velocity;
	positionOld = position;

	// gravity
	velocity = velocityOld + world->getGravity() * step;

	// update position
	if (position.y > 0)
	{
		position = positionOld + velocityOld*step;
	}
	else
	{
		velocity = glm::vec2(0.0f);
	}
}

void PTRigidBody::setPosition(glm::vec2& pos)
{
	position = pos;
}

glm::vec2 PTRigidBody::getPosition() const
{
	return position;
}

void PTRigidBody::setVelocity(glm::vec2& veloc)
{
	velocity = veloc;
}