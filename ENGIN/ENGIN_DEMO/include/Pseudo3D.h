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

#include <array>

namespace Engin
{
	namespace Game
	{
		class Pseudo3D : public Scene
		{
		public:
			Pseudo3D(Engin* engine);
			~Pseudo3D();

			void update(GLfloat step);
			void interpolate(GLfloat alpha);
			void draw();
			void renderTexture(Resources::Texture* texture, float x, float y, const Renderer::Camera& camera);
		
			void emptyVector(int vectorAsNumber);
			void addIntoVector(int vectorAsNumber, glm::vec2 xy, int tiletype);

			void DDA();
			void DDADrawSprites();
			
		private:
			Engin* engine;

			Renderer::Camera camera;
			Renderer::Camera camera2;
			Renderer::Camera camera3; //HUD

			Resources::ShaderProgram* shader;
			Resources::ShaderProgram* textureShader;
			Resources::ShaderProgram* alphaShader;
			Renderer::Batch batch;
			Renderer::TextureBatch textureBatch;
			Renderer::TextureBatch alphaTextureBatch;
			Renderer::TextureBatch alphaTextureBatch2;

			Resources::Texture* doge1;
			Resources::Texture* doge2;
			Resources::Texture* doge3;
			Resources::Texture* doge4;
			Resources::Texture* doge5;
			Resources::Texture* furball;
			Resources::Texture* text;
			Resources::Texture* text2;
			Resources::Texture* text3;
			Renderer::TextRenderer textCreator;
			Renderer::TextRenderer textCreator3;
			Resources::Font* font;
			Core::Timer myTimer;

			std::vector<int> objectTiles;
			std::vector<glm::vec2> spriteContainer;
			glm::vec2 player;
			glm::vec2 sprite;
			
			float alpha;

			int endX;
			int visibleTilesCount;

			int mapX;
			int mapY;
			int tileSize;

			//DDA
			int w;
			int h;
			double dirX, dirY; //initial direction vector
			double planeX, planeY; //the 2d raycaster version of camera plane

			int DDAX,DDAY;

			int DDAtexture;
			int drawColor;
			std::vector<glm::vec4> DDAlines;
			std::vector<glm::vec4> DDASpriteDrawData;
			std::vector<int> DDAlinesTexX;
			double depth;

			float moveSpeed;
			float rotSpeed;			
		};
	}
}
