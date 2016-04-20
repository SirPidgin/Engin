#include "PTRigidBody.h"

PTRigidBody::PTRigidBody(PTPhysicsWorld* world) : world(world), colliding(false), rotation(0), angularVelocity(0)
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
	
	position = positionOld + velocity*step;

	velocity = velocity + world->getGravity() * step / 2.0f;

	// Torque
	rotation = rotationOld + angularVelocity*step;
}

void PTRigidBody::setPosition(glm::vec2& pos)
{
	position = pos;
}

glm::vec2 PTRigidBody::getPosition() const
{
	return position;
}

void PTRigidBody::setRotation(GLfloat rotation)
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

glm::vec2& PTRigidBody::getVelocity()
{
	return velocity;
}

// TODO make one init...
void PTRigidBody::setHalfSize(glm::vec2& halfSize)
{
	this->halfSize = halfSize;

	I = (1.0f / 12.0f) * mass * (halfSize.x * halfSize.x + halfSize.y * halfSize.y);
}

glm::vec2 PTRigidBody::getHalfSize()
{
	return halfSize;
}

void PTRigidBody::setAngularVelocity(GLfloat& velocity)
{
	angularVelocity = velocity;
}

GLfloat& PTRigidBody::getAngularVelocity()
{
	return angularVelocity;
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

void PTRigidBody::setMass(GLfloat mass)
{
	this->mass = mass;
}

GLfloat PTRigidBody::getMass()
{
	return mass;
}

GLfloat PTRigidBody::getI()
{
	return I;
}