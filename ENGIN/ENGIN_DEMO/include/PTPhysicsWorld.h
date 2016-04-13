#pragma once

#include "GL\glew.h"
#include "glm\glm.hpp"
#include <vector>
#include "PTRigidBody.h"

class PTRigidBody;

class PTPhysicsWorld
{
public:
	PTPhysicsWorld(GLfloat scale);
	~PTPhysicsWorld();

	void update(GLfloat step);
	void addRigidBody(PTRigidBody* body, glm::vec2& position);

	void setGravity(glm::vec2 gravity);
	glm::vec2 getGravity();

	std::vector<PTRigidBody*> getBodies();

	bool isCollidingAABB(PTRigidBody* body1, PTRigidBody* body2);

private:
	glm::vec2 gravity;
	std::vector<PTRigidBody*> rigidBodies;
	GLfloat scale;
};