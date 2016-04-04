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

#include <array>
#include <vector>

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
			GameObject* gameObject;

			std::vector<GameObject*> gameObjects;
		};
	}
}
