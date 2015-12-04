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
#include "Engin\Core\RNG.h"
#include "Engin\Renderer\TextRenderer.h"
#include "Engin\Resources\Font.h"
#include "Engin\Game\AnimationPlayer.h"
#include "Engin\Resources\Animation.h"
#include "Engin\Game\GameObject.h"
#include "Engin\Game\Sprite.h"
#include "Engin\Game\Component.h"
#include "Engin\Core\RNG.h"

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

			void Raycasting();
			void RaycastingSprites();
			void DrawRaycastLines();
			void Draw2dVision();

			void createFurball(float x, float y, float rotation);
			void createFireball(float x, float y, float rotation);
			void createProjectile(float x, float y, float rotation);
			void createHitFireball(float x, float y);

			class Projectile : public Component
			{
			public:
				Projectile(GameObject* o) : Component(o) {}
				void update();

			private:
				float speed = 0.10f;
				float spread = 5.0f;
				Core::RNG rng; // Should be static?
			};
			void createTree(float x, float y, float rotation);

			class UserData : public Component
			{
			public:
				UserData(GameObject* o) : Component(o){}
				int sides; 
				double transformY;
				int animationIndex;
				double spriteXout;
				double spriteYout;
				bool isFireball = false;
				bool hasShadow = false;
				int animationLoopStartFrame = 0;
				int animationLoopEndFrame = 0;
				Resources::Texture* shadow;
				Core::Timer hitCoolDown;
				float cooldownLenght = 0.0f;
				int tileOverSize = 0;
				bool isTree = false;
				float depthRandom = 0.0f;
				float spriteColorR;
				float spriteColorG;
				float spriteColorB;
				bool isHitAnimation = false;
			};

			class PseudoSpriteDraw : public Component
			{
			public:
				PseudoSpriteDraw(GameObject* o) : Component(o){}
				void setTextureBatch(Renderer::TextureBatch* newTextrBatch) { textureBatch = newTextrBatch; }
				void setRaycastW(int W) { raycastW = W; limitLeft = -raycastW - 2656; limitRight = raycastW - 2400; }
				void drawPseudoSprite()				   
				{					
					
					static float colorValue = 0;

					if (ownerObject->accessComponent<UserData>()->isFireball || ownerObject->accessComponent<UserData>()->isHitAnimation)
					{
						colorValue = 1.0f;
					}
					else
					{
						colorValue = (ownerObject->accessComponent<Transform>()->getDepth() + ownerObject->accessComponent<UserData>()->depthRandom) * 6;
						if (colorValue > 0.8f)
						{
							colorValue = 0.8f;
						}
						else if (colorValue < 0.4f)
						{
							colorValue = 0.4f;
						}
					}				

					if (ownerObject->accessComponent<UserData>()->spriteXout > limitLeft
						&& ownerObject->accessComponent<UserData>()->spriteXout < limitRight
						&& ownerObject->accessComponent<UserData>()->transformY > 0)
					{
						textureBatch->draw(ownerObject->accessComponent<AnimationPlayer>()->getTexture(), ownerObject->accessComponent<AnimationPlayer>()->getCurrentFrameTexCoords(),
							ownerObject->accessComponent<UserData>()->spriteXout, ownerObject->accessComponent<UserData>()->spriteYout,
							ownerObject->accessComponent<AnimationPlayer>()->getFrameWidth(), ownerObject->accessComponent<AnimationPlayer>()->getFrameHeight(),
							0.0f, 0.0f,	0.0f, 
							ownerObject->accessComponent<Transform>()->getScale(), 
							Renderer::Color{ ownerObject->accessComponent<UserData>()->spriteColorR, ownerObject->accessComponent<UserData>()->spriteColorG, ownerObject->accessComponent<UserData>()->spriteColorB } *colorValue,
							1.0f, ownerObject->accessComponent<Transform>()->getDepth() + ownerObject->accessComponent<UserData>()->depthRandom);

						//shadow
						if (ownerObject->accessComponent<UserData>()->hasShadow == true)
						{
							textureBatch->draw(ownerObject->accessComponent<UserData>()->shadow, &glm::vec4(0.0f, 0.0f, ownerObject->accessComponent<UserData>()->shadow->getWidth(), ownerObject->accessComponent<UserData>()->shadow->getHeight()),
								ownerObject->accessComponent<UserData>()->spriteXout, ownerObject->accessComponent<UserData>()->spriteYout,
								ownerObject->accessComponent<UserData>()->shadow->getWidth(), (ownerObject->accessComponent<UserData>()->shadow->getHeight() + ownerObject->accessComponent<Transform>()->getDepth()*50),
								0.0f, 0.0f, 0.0f,
								ownerObject->accessComponent<Transform>()->getScale(), 
								Renderer::Color{ ownerObject->accessComponent<UserData>()->spriteColorR, ownerObject->accessComponent<UserData>()->spriteColorG, ownerObject->accessComponent<UserData>()->spriteColorB } *colorValue, 1.0f,
								ownerObject->accessComponent<Transform>()->getDepth() - 0.00001 + ownerObject->accessComponent<UserData>()->depthRandom);
						}						
					}
				}

			private:
				Renderer::TextureBatch* textureBatch;
				int raycastW;
				int limitLeft;
				int limitRight;
			};
			
		private:
			Core::RNG randomGenerator;

			// Private methods.
			int getSpriteAnimIndex(double angle, double sides);
			void deleteDeadObjects();
			float getAxisMultiplier(HID::GamepadAxis axis, int GPIndex);
			void movePlayer(float multiplier);
			void strafePlayer(float multiplier);
			void rotatePlayer(float speed);

			Engin* engine;

			Renderer::Camera* camera;
			Renderer::Camera* camera2;
			Renderer::Camera* camera3; //HUD

			Resources::ShaderProgram* shader;
			Resources::ShaderProgram* textureShader;
			Resources::ShaderProgram* alphaShader;

			Resources::Texture* furball; //2d
			Resources::Texture* tree_64; //2d
			Resources::Texture* mapSheet_64;
			Resources::Texture* mapSheet_256;
			Resources::Texture* floor_16;
			Resources::Texture* floor_800;

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
			std::vector<GameObject*> deadObjects;
			
			float alpha;

			int endX;
			int visibleTilesCount;

			int mapX;
			int mapY;
			int tileSize;
			int tileSize2d;

			bool useGamePad;

			//turret timer
			Core::Timer turretCoolDown;

			//-------------------------------------------------------------------
			//Raycasting
			Resources::Animation* animFurball360;
			Resources::Texture* furballShadow;
			Resources::Animation* animFireball360;
			Resources::Animation* animTree360;
			Resources::Texture* treeShadow;
			AnimationPlayer animPlayer2d;
			Resources::Animation* animFurballHit;

			int raycastW;
			int raycastH;
			double dirX, dirY; //initial direction vector
			double planeX, planeY; //the 2d raycaster version of camera plane

			int raycastX, raycastY;

			int raycastTileIndex;
			std::array<std::array<double,5>, 800> Raycastlines; //Size has to be same as raycastW
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
			glm::vec2 transform;
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
