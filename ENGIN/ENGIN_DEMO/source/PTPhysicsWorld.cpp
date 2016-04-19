#include "PTPhysicsWorld.h"
#include "glm\gtc\matrix_transform.hpp"

PTPhysicsWorld::PTPhysicsWorld(GLfloat scale) : scale(scale), e(0.15)
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
				isCollidingSAT(rigidBodies[i], rigidBodies[j]);
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

bool PTPhysicsWorld::isCollidingSAT(PTRigidBody* body1, PTRigidBody* body2)
{
	// On kevyt...

	// first
	glm::mat4 T = glm::translate(glm::mat4(), glm::vec3(body1->getPosition().x, body1->getPosition().y, 1.0f));
	glm::mat4 R = glm::rotate(glm::mat4(), body1->getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 T_1 = glm::translate(glm::mat4(), glm::vec3(-body1->getPosition().x, -body1->getPosition().y, 1.0f));
	glm::mat4 muunnos = T*R*T_1;

	glm::vec4 temp = (muunnos*glm::vec4(body1->getPosition().x - body1->getHalfSize().x, body1->getPosition().y + body1->getHalfSize().y, 0.0f, 1.0f));
	glm::vec2 topLeft = glm::vec2(temp.x, temp.y);

	temp = (muunnos*glm::vec4(body1->getPosition().x + body1->getHalfSize().x, body1->getPosition().y + body1->getHalfSize().y, 0.0f, 1.0f));
	glm::vec2 topRight = glm::vec2(temp.x, temp.y);

	temp = (muunnos*glm::vec4(body1->getPosition().x + body1->getHalfSize().x, body1->getPosition().y - body1->getHalfSize().y, 0.0f, 1.0f));
	glm::vec2 bottomRight = glm::vec2(temp.x, temp.y);

	temp = (muunnos*glm::vec4(body1->getPosition().x - body1->getHalfSize().x, body1->getPosition().y - body1->getHalfSize().y, 0.0f, 1.0f));
	glm::vec2 bottomLeft = glm::vec2(temp.x, temp.y);

	glm::vec2 box1_point[]{topLeft, topRight, bottomRight, bottomLeft };

	// second
	T = glm::translate(glm::mat4(), glm::vec3(body2->getPosition().x, body2->getPosition().y, 1.0f));
	R = glm::rotate(glm::mat4(), body2->getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
	T_1 = glm::translate(glm::mat4(), glm::vec3(-body2->getPosition().x, -body2->getPosition().y, 1.0f));
	muunnos = T*R*T_1;

	temp = (muunnos*glm::vec4(body2->getPosition().x - body2->getHalfSize().x, body2->getPosition().y + body2->getHalfSize().y, 0.0f, 1.0f));
	glm::vec2 topLeft1 = glm::vec2(temp.x, temp.y);

	temp = (muunnos*glm::vec4(body2->getPosition().x + body2->getHalfSize().x, body2->getPosition().y + body2->getHalfSize().y, 0.0f, 1.0f));
	glm::vec2 topRight1 = glm::vec2(temp.x, temp.y);

	temp = (muunnos*glm::vec4(body2->getPosition().x + body2->getHalfSize().x, body2->getPosition().y - body2->getHalfSize().y, 0.0f, 1.0f));
	glm::vec2 bottomRight1 = glm::vec2(temp.x, temp.y);

	temp = (muunnos*glm::vec4(body2->getPosition().x - body2->getHalfSize().x, body2->getPosition().y - body2->getHalfSize().y, 0.0f, 1.0f));
	glm::vec2 bottomLeft1 = glm::vec2(temp.x, temp.y);

	glm::vec2 box2_point[]{topLeft1, topRight1, bottomRight1, bottomLeft1 };


	float box1_min = 0;
	float box1_max = 0;
	float box2_min = 0;
	float box2_max = 0;


	glm::vec2 normal;
	glm::vec2 lineSlope;

	for (int i = 0; i < 8; i++)
	{
		box1_min = 0;
		box1_max = 0;
		box2_min = 0;
		box2_max = 0;

		if (i < 4)
		{
			int q = i + 1;
			if (q >= 4)
				q = 0;

			lineSlope = slope(box1_point[i], box1_point[q]);

			normal = glm::vec2(-lineSlope.y, lineSlope.x);
		}
		else
		{
			int q = i - 4 + 1;
			if (q >= 4)
				q = 0;
			lineSlope = slope(box2_point[i - 4], box2_point[q]);

			normal = glm::vec2(-lineSlope.y, lineSlope.x);
		}


		box1_max = projectedPoint(box1_point[0], normal);
		box1_min = projectedPoint(box1_point[0], normal);

		box2_max = projectedPoint(box2_point[0], normal);
		box2_min = projectedPoint(box2_point[0], normal);

		for (int p = 0; p < 8; p++)
		{
			if (p < 4)
			{
				if (projectedPoint(box1_point[p], normal) > box1_max)
					box1_max = projectedPoint(box1_point[p], normal);

				else if (projectedPoint(box1_point[p], normal) < box1_min)
					box1_min = projectedPoint(box1_point[p], normal);
			}

			else
			{
				if (projectedPoint(box2_point[p - 4], normal) > box2_max)
					box2_max = projectedPoint(box2_point[p - 4], normal);

				else if (projectedPoint(box2_point[p - 4], normal) < box2_min)
					box2_min = projectedPoint(box2_point[p - 4], normal);
			}
		}

		if (box2_min > box1_max || box1_min > box2_max)
		{

			return false;
		}

	}

	collisionResolution(box1_point, box2_point, body1, body2);
	return true;
}

void PTPhysicsWorld::collisionResolution(glm::vec2 box1_point[], glm::vec2 box2_point[], PTRigidBody* body1, PTRigidBody* body2)
{
	for (int i = 0; i < box1_point->length(); i++)
	{
		if (pointInside(box1_point[i], box2_point))
		{
			//box1 point collidies with box2
			// TODO laske normaali
			glm::vec2 n = glm::normalize(glm::vec2(glm::cos(body2->getRotation()), glm::sin(body2->getRotation())));
			//TODO tarkista rAP ja rBP
			glm::vec2 rAP = glm::vec2(box1_point[i].x - body1->getPosition().x, box1_point[i].y - body1->getPosition().y);
			glm::vec2 r_AP = glm::vec2(-rAP.y, rAP.x);

			glm::vec2 rBP = glm::vec2(box1_point[i].x - body2->getPosition().x, box1_point[i].y - body2->getPosition().y);
			glm::vec2 r_BP = glm::vec2(-rBP.y, rBP.x);

			GLfloat J = calculateJ(box1_point[i], body1, body2, n, r_AP, r_BP);

			glm::vec2 newVeloc = body1->getVelocity() + (J / body1->getMass())*n;
			body1->setVelocity(newVeloc);

			GLfloat newAngularVelocity = body1->getAngularVelocity() + (glm::dot(r_AP, J*n) / body1->getI());
			body1->setAngularVelocity(newAngularVelocity);

			glm::vec2 newVeloc2 = body2->getVelocity() + (-J / body2->getMass())*n;
			body2->setVelocity(newVeloc2);

			GLfloat newAngularVelocity2 = body2->getAngularVelocity() + (glm::dot(r_BP, -J*n) / body2->getI());
			body2->setAngularVelocity(newAngularVelocity2);

			body1->setCollisionOn();
			body2->setCollisionOn();
		}
	}

	for (int i = 0; i < box2_point->length(); i++)
	{
		if (pointInside(box2_point[i], box1_point))
		{
			//box2 point collidies with box1
			// TODO laske normaali
			glm::vec2 n = glm::normalize(glm::vec2(glm::cos(body1->getRotation()), glm::sin(body1->getRotation())));
			//TODO tarkista rAP ja rBP
			glm::vec2 rAP = glm::vec2(box2_point[i].x - body1->getPosition().x, box2_point[i].y - body1->getPosition().y);
			glm::vec2 r_AP = glm::vec2(-rAP.y, rAP.x);

			glm::vec2 rBP = glm::vec2(box2_point[i].x - body2->getPosition().x, box2_point[i].y - body2->getPosition().y);
			glm::vec2 r_BP = glm::vec2(-rBP.y, rBP.x);

			GLfloat J = calculateJ(box2_point[i], body1, body2, n, r_AP, r_BP);

			glm::vec2 newVeloc = body1->getVelocity() + (-J / body1->getMass())*n;
			body1->setVelocity(newVeloc);

			GLfloat newAngularVelocity = body1->getAngularVelocity() + (glm::dot(r_AP, -J*n) / body1->getI());
			body1->setAngularVelocity(newAngularVelocity);

			glm::vec2 newVeloc2 = body2->getVelocity() + (J / body2->getMass())*n;
			body2->setVelocity(newVeloc2);

			GLfloat newAngularVelocity2 = body2->getAngularVelocity() + (glm::dot(r_BP, J*n) / body2->getI());
			body2->setAngularVelocity(newAngularVelocity2);

			body1->setCollisionOn();
			body2->setCollisionOn();
		}
	}
}

bool PTPhysicsWorld::pointInside(glm::vec2 P, glm::vec2 box_point[])
{
	int i;
	int j;

	int maxSides = 4;

	for (i = 0, j = maxSides - 1; i < maxSides; j = i++) {
		if (((box_point[i].y > P.y) != (box_point[j].y > P.y)) && (P.x < (box_point[j].x - box_point[i].x) * (P.y - box_point[i].y) / (box_point[j].y - box_point[i].y) + box_point[i].x))
		{
			return true;
		}
	}
	return false;
}

GLfloat PTPhysicsWorld::calculateJ(glm::vec2 colPoint, PTRigidBody* body1, PTRigidBody* body2, glm::vec2 n, glm::vec2 r_AP, glm::vec2 r_BP)
{
	GLfloat J;

	glm::vec2 vAP = body1->getVelocity() + body1->getAngularVelocity()*r_AP;
	glm::vec2 vBP = body2->getVelocity() + body2->getAngularVelocity()*r_BP;

	glm::vec2 vAB = vAP - vBP;

	J = (-(1.0f + e) * glm::dot(vAB, n)) / (glm::dot(n, n)*(1.0f / body1->getMass() + 1.0f / body2->getMass()) + ((glm::dot(r_AP, n))*(glm::dot(r_AP, n)) / body1->getI()) + (((glm::dot(r_BP, n))*(glm::dot(r_BP, n))) / body2->getI()));

	return J;
}