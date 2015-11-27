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
#include "Engin\Game\AnimationPlayer.h"
#include "Engin\Resources\Animation.h"
#include "Engin\Game\GameObject.h"
#include <Engin\Game\Sprite.h>
#include "Engin\Game\Component.h"

#include <array>
#include <vector>

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
			void addIntoVector(int vectorAsNumber, glm::vec2 xy, int tiletype);

			void Raycasting();
			void RaycastingSprites();

			void createFurball(float x, float y, float rotation);
			void createFireball(float x, float y, float rotation);

			class UserData : public Component
			{
			public:
				UserData(GameObject* o) : Component(o){}
				int sides; 
				double transformY;
				int animationIndex;
				int raycastW = 400;
				int tileSize = 256;
				double spriteXout;
				double spriteYout;
				bool isFireball = false;
				int animationLoopStartFrame = 0;
				int animationLoopEndFrame = 0;
			};

			class PseudoSpriteDraw : public Component
			{
			public:
				PseudoSpriteDraw(GameObject* o) : Component(o){}
				void setTextureBatch(Renderer::TextureBatch* newTextrBatch) { textureBatch = newTextrBatch; }
				void drawPseudoFurball()
				{					
					if (ownerObject->accessComponent<UserData>()->spriteXout > -ownerObject->accessComponent<UserData>()->raycastW -1600
						&& ownerObject->accessComponent<UserData>()->spriteXout < (ownerObject->accessComponent<UserData>()->raycastW -1600 + ownerObject->accessComponent<UserData>()->tileSize) 
						&& ownerObject->accessComponent<UserData>()->transformY > 0)
					{
						textureBatch->draw(ownerObject->accessComponent<AnimationPlayer>()->getTexture(), ownerObject->accessComponent<AnimationPlayer>()->getCurrentFrameTexCoords(),
							ownerObject->accessComponent<UserData>()->spriteXout, ownerObject->accessComponent<UserData>()->spriteYout,
							ownerObject->accessComponent<AnimationPlayer>()->getFrameWidth(), ownerObject->accessComponent<AnimationPlayer>()->getFrameHeight(),
							0.0f, 0.0f,	0.0f, 
							ownerObject->accessComponent<Transform>()->getScale(),
							Renderer::clrWhite, 1.0f, ownerObject->accessComponent<Transform>()->getDepth());
					}
				}

			private:
				Renderer::TextureBatch* textureBatch;
			};
			
		private:
			int getSpriteAnimIndex(double angle, double sides);
			Engin* engine;

			Renderer::Camera* camera;
			Renderer::Camera* camera2;
			Renderer::Camera* camera3; //HUD

			Resources::ShaderProgram* shader;
			Resources::ShaderProgram* textureShader;
			Resources::ShaderProgram* alphaShader;

			Resources::Texture* furball; //2d
			Resources::Texture* mapSheet_64;
			Resources::Texture* mapSheet_256;
			Resources::Texture* roof_16;

			Resources::Texture* text;
			Resources::Texture* text2;
			Resources::Texture* text3;
			Renderer::TextRenderer textCreator;
			Renderer::TextRenderer textCreator3;
			Resources::Font* font;
			Core::Timer myTimer;

			std::array<std::array<int,25>,25> wallTiles; //Notice the world size mapX and mapY
			std::array<double,5> player;

			std::vector<GameObject*> gameObjects;
			
			float alpha;

			int endX;
			int visibleTilesCount;

			int mapX;
			int mapY;
			int tileSize;
			int tileSize2d;

			//Raycasting
			Resources::Animation* animFurball360;
			Resources::Animation* animFireball360;
			AnimationPlayer animPlayer2d;

			int raycastW;
			int raycastH;
			double dirX, dirY; //initial direction vector
			double planeX, planeY; //the 2d raycaster version of camera plane

			int raycastX, raycastY;

			int raycastTileIndex;
			std::array<std::array<double,5>, 400> DDAlines; //change DDAlines size accordingly
			int spriteAnimIndex;
			double depth;

			double cameraX; //x-coordinate in camera space
			double rayPosX;
			double rayPosY;
			double rayDirX;
			double rayDirY;

			//length of ray from current position to next x or y-side
			double sideDistX;
			double sideDistY;

			//length of ray from one x or y-side to next x or y-side
			double deltaDistX;
			double deltaDistY;
			double perpWallDist;

			//what direction to step in x or y-direction (either +1 or -1)
			int stepX;
			int stepY;

			int hit; //was there a wall hit?
			int side; //was a NS or a EW wall hit?

			//Calculate height of line to draw on screen
			int lineHeight;

			//calculate lowest and highest pixel to fill in current stripe
			int drawStart;			
			int drawEnd;

			double wallX; //where exactly the wall was hit
			int texX;
			int texNum;

			double spriteX;
			double spriteY;
			double spriteXout;
			double spriteYout;
			double spriteScreenX;
			double spriteScale;
			double spriteHeightWidth;
			double spriteFacing;
			double spriteAngle;
			float spriteSideAngle;
			float convertToFloat;

			//for "3d" sprite animation in draw
			int spriteStartFrame;
			int spriteEndFrame;

			float moveSpeed;
			float rotSpeed;
		};
	}
}
