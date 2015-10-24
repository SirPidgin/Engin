#pragma once

#include "Engin\Game\Scene.h"
#include <Engin\Renderer\Camera.h>
#include <Engin\Renderer\Batch.h>

namespace Engin
{
	namespace Game
	{
		class CameraTestScene : public Scene
		{
		public:
			CameraTestScene();
			~CameraTestScene();

			void update(GLfloat alpha);
			void draw();
			void renderTilemap(float startX, float startY, float tileWidth, float tileHeight, size_t worldWidth, size_t worldHeight, Renderer::Batch& batch);

		public:
			GLfloat alpha;

			Renderer::Camera camera;
			Renderer::Camera camera1;
			Renderer::Camera camera2;

			Resources::Shader shader;
			Renderer::Batch batch;
		};
	}
}
