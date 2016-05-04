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

	GLfloat getScale()
	{
		return scale;
	}

private:
	glm::vec2 gravity;
	std::vector<PTRigidBody*> rigidBodies;
	GLfloat scale;
	float epsi;

	glm::mat4 T;
	glm::mat4 R1;
	glm::mat4 T_1;
	glm::mat4 muunnos;

	glm::vec4 temp;
	glm::vec2 topLeft;

	glm::vec2 topRight;

	glm::vec2 bottomRight;

	glm::vec2 bottomLeft;

	glm::vec2 box1_point[4];

	glm::mat4 R2;

	glm::vec2 topLeft1;

	glm::vec2 topRight1;

	glm::vec2 bottomRight1;

	glm::vec2 bottomLeft1;

	glm::vec2 box2_point[4];

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

	GLfloat calculateJ(glm::vec2 colPoint, PTRigidBody* body1, PTRigidBody* body2, glm::vec2 n, glm::vec2 r_AP, glm::vec2 r_BP, glm::vec2 vAB);

	bool areLinesIntersecting(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D);

	GLfloat PerpDot(glm::vec2 PointA, glm::vec2 PointB)                    
	{ 
		return (PointA.y*PointB.x) - (PointA.x*PointB.y);
	}

	glm::vec2 colNormal;

	bool wasHitCalc(glm::vec2 box1_point[], int index, glm::vec2 box2_point[], PTRigidBody* body1, PTRigidBody* body2, int maxSides);
};