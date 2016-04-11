#pragma once
#include "glm\glm.hpp"
#include "GL\glew.h"
#include "PTPhysicsWorld.h"

class PTPhysicsWorld;

class PTRigidBody
{
public:
	PTRigidBody(PTPhysicsWorld* world);
	~PTRigidBody();

	void update(GLfloat step);

	void setPosition(glm::vec2 &position);
	glm::vec2 getPosition() const;

	void setVelocity(glm::vec2& velocity);
private:
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 positionOld;
	glm::vec2 velocityOld;
	glm::vec2 angularVelocity;

	PTPhysicsWorld* world;

	glm::vec2 size;
};