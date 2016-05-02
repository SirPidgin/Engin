#pragma once

#include <Engin\Engin.h>
#include "Engin\Game\Scene.h"
#include <Engin\Renderer\Camera.h>
#include <Engin\Renderer\Batch.h>
#include "Engin\Resources\ResourceManager.h"
#include "Engin\Resources\Texture.h"
#include "Engin\Resources\ShaderProgram.h"
#include "Engin\Game\GameObject.h"
#include "Engin\Game\Sprite.h"
#include "Engin\Game\Component.h"
#include "Engin\Core\RNG.h"
#include <array>
#include <vector>

#include "PTPhysicsWorld.h"
#include "PTRigidBody.h"

#include "Engin\Core\Timer.h""

namespace Engin
{
	namespace Game
	{
		class PhysicsDemo : public Scene
		{
		public:
			PhysicsDemo(Engin* engine);
			~PhysicsDemo();

			void update(GLfloat step);
			void interpolate(GLfloat alpha);
			void draw();

			void createTile(GLfloat x, GLfloat y, GLfloat r);
			void createTile256(GLfloat x, GLfloat y, GLfloat r);
			void cameraMovement(GLfloat step);

			void fireTile(GLfloat x, GLfloat y, GLfloat r, glm::vec2 veloc);

			void deleteDeadObjects();

		private:
			GLfloat alpha;
			Resources::ShaderProgram* shader;
			Resources::ShaderProgram* textureShader;
			Resources::ShaderProgram* alphaShader;
			Renderer::Batch batch;
			Renderer::TextureBatch textureBatch;
			Renderer::TextureBatch alphaTextureBatch;
	
			Engin* engine;

			Renderer::Camera* camera;
			bool useGamePad;

			Resources::Texture* tile;
			Resources::Texture* tile2;
			Resources::Texture* tile256;
			GameObject* gameObject;

			std::vector<GameObject*> gameObjects;
			std::vector<GameObject*> deadObjects;

			PTPhysicsWorld* physicsWorld;

			Core::Timer* fireTimer;
			Core::RNG randomGenerator;

			bool firstTime;
			bool secondTime;

			GLfloat timeToShoot;
			int maxObjects;
		};

		class PhysicsComponent : public Component
		{
		public:
			PhysicsComponent(GameObject* o) : Component(o){}
			~PhysicsComponent()
			{
				body->kill();
			}
			void setBody(PTRigidBody* body)
			{
				this->body = body;
			}
			PTRigidBody* getBody()
			{
				return body;
			}
			void update()
			{
				if (body->getPosition().y < -400)
				{
					ownerObject->kill();
				}
				ownerObject->accessComponent<Transform>()->setPosition(body->getPosition());
				ownerObject->accessComponent<Transform>()->setRotation(body->getRotation());
			}
		private:
			PTRigidBody* body;
		};
	}
}
