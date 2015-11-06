#pragma once

#include "Engin\Game\Scene.h"
#include <Engin\Renderer\Camera.h>
#include <Engin\Renderer\Batch.h>
#include <Engin\Renderer\TextureBatch.h>
#include "Engin\Resources\ResourceManager.h"
#include "Engin\Resources\Texture.h"

namespace Engin
{
	namespace Game
	{
		class CameraTestScene : public Scene
		{
		public:
			CameraTestScene();
			~CameraTestScene();

			void update(GLfloat step);
			void interpolate(GLfloat alpha);
			void draw();
			void renderTilemap(float startX, float startY, float tileWidth, float tileHeight, size_t worldWidth, size_t worldHeight, Renderer::Batch& batch);
			void renderDogemap(float startX, float startY, float tileWidth, float tileHeight, size_t worldWidth, size_t worldHeight);
			void renderTexture(Resources::Texture* texture, float x, float y, const Renderer::Camera& camera);
		public:
			GLfloat alpha;

			Renderer::Camera camera;
			Renderer::Camera camera1;
			Renderer::Camera camera2;

			Resources::ShaderProgram shader;
			Resources::ShaderProgram textureShader;
			Resources::ShaderProgram alphaShader;
			Renderer::Batch batch;
			Renderer::TextureBatch textureBatch;
			Renderer::TextureBatch alphaTextureBatch;

			Resources::ResourceManager resourceManager;
			Resources::Texture* doge;
		};
	}
}
