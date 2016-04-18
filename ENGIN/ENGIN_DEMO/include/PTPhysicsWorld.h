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



private:
	glm::vec2 gravity;
	std::vector<PTRigidBody*> rigidBodies;
	GLfloat scale;

	bool isCollidingSAT(PTRigidBody* body1, PTRigidBody* body2);

	void collisionResolution(glm::vec2 box1_point[], glm::vec2 box2_point[], PTRigidBody* body1, PTRigidBody* body2);

	float projectedPoint(glm::vec2 point, glm::vec2 slope)
	{
		return (point.x * slope.x) + (point.y * slope.y);
	}

	glm::vec2 slope(glm::vec2 P1, glm::vec2 P2)
	{
		return glm::vec2((P2.x - P1.x), (P2.y - P1.y));
	}

	bool pointInside(glm::vec2 P, glm::vec2 box_point[]);
};