#include "PTRigidBody.h"

PTRigidBody::PTRigidBody(PTPhysicsWorld* world) : world(world), colliding(false)
{

}

PTRigidBody::~PTRigidBody()
{

}

void PTRigidBody::update(GLfloat step)
{
	positionOld = position;
	rotationOld = rotation;

	// gravity
	velocity = velocity + world->getGravity() * step / 2.0f;

	// update position
	if (position.y > 0)
	{
		position = positionOld + velocity*step;
	}
	else
	{
		velocity.y = 0.0f;
		position.y = 0.0f;
		position = positionOld + velocity*step;
	}

	velocity = velocity + world->getGravity() * step / 2.0f;

	// Torque
	
}

void PTRigidBody::setPosition(glm::vec2& pos)
{
	position = pos;
}

glm::vec2 PTRigidBody::getPosition() const
{
	return position;
}

void PTRigidBody::setRotation(GLfloat& rotation)
{
	this->rotation = rotation;
}

GLfloat PTRigidBody::getRotation() const
{
	return rotation;
}

void PTRigidBody::setVelocity(glm::vec2& veloc)
{
	velocity = veloc;
}

void PTRigidBody::setHalfSize(glm::vec2& halfSize)
{
	this->halfSize = halfSize;
}

glm::vec2 PTRigidBody::getHalfSize()
{
	return halfSize;
}

void PTRigidBody::setCollisionOn()
{
	colliding = true;
}
void PTRigidBody::setCollisionOff()
{
	colliding = false;
}

bool PTRigidBody::isColliding()
{
	return colliding;
}