#pragma once

#include <Engin\Engin.h>
#include "Engin\Game\Scene.h"
#include <Engin\Renderer\Camera.h>
#include <Engin\Renderer\Batch.h>
#include <Engin\Renderer\TextureBatch.h>
#include "Engin\Resources\ResourceManager.h"
#include "Engin\Resources\Texture.h"
#include "Engin\Resources\ShaderProgram.h"
#include "Engin\Core\Timer.h"
#include "Engin\Renderer\TextRenderer.h"
#include "Engin\Resources\Font.h"

namespace Engin
{
	namespace Game
	{
		class CameraTestScene : public Scene
		{
		public:
			CameraTestScene(Engin* engine);
			~CameraTestScene();

			void update(GLfloat step);
			void interpolate(GLfloat alpha);
			void draw();
			void renderDogemap(float startX, float startY, float tileWidth, float tileHeight, size_t worldWidth, size_t worldHeight);
			void renderTexture(Resources::Texture* texture, float x, float y, const Renderer::Camera& camera);
		
			glm::vec2 inputSwap(glm::vec2 xy, int octant);
			glm::vec2 outputSwap(glm::vec2 xy, int octant);
			void plotLine(glm::vec2 point0, glm::vec2 point1);
			int calculateOctant(glm::vec2 point0, glm::vec2 point1);
			bool addVisiblePoint(glm::vec2 point);

			void calculateVision(int playerX, int playerY);
			void calculate90(int playerX, int playerY);

		private:
			Engin* engine;

			Renderer::Camera camera;

			Resources::ShaderProgram* shader;
			Resources::ShaderProgram* textureShader;
			Resources::ShaderProgram* alphaShader;
			Renderer::Batch batch;
			Renderer::TextureBatch textureBatch;
			Renderer::TextureBatch alphaTextureBatch;

			Resources::Texture* doge;
			Resources::Texture* doge2;
			Resources::Texture* doge3;
			Resources::Texture* doge4;
			Resources::Texture* doge5;
			Resources::Texture* text;
			Resources::Texture* text2;
			Resources::Texture* text3;
			Renderer::TextRenderer textCreator;
			Renderer::TextRenderer textCreator2;
			Renderer::TextRenderer textCreator3;
			Resources::Font* font;
			Core::Timer myTimer;
			Core::Timer myTimer2;

			//algorithm
			glm::vec2 point0;
			glm::vec2 point1;
			glm::vec2 temp0;
			glm::vec2 temp1;
			glm::vec2 temp;
			std::vector<glm::vec2> visibleTiles;
			std::vector<glm::vec2> visibleFriend;
			std::vector<glm::vec2> wallTiles;
			int Difference;
			int dx;
			int dy;
			int y;
			float m;
			int octant;
			float alpha;

			
			int playerX;
			int playerY;

			int friendX;
			int friendY;
			
			int endX;
		};
	}
}
