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
			void addIntoVector(int vectorAsNumber, glm::vec2 xy, int tiletype);

			void DDA();
			void DDADrawSprites();
			
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

			std::array<std::array<int,25>,25> objectTiles; //Notice the world size mapX and mapY
			std::vector<std::array<double, 5>> spriteContainer;
			std::array<double,5> player;
			std::array<double,5> sprite;
			std::array<double,5> sprite1;
			std::array<double,5> sprite2;
			std::array<double,5> sprite3;
			std::array<double,5> sprite4;
			std::array<double,5> sprite5;
			std::array<double,5> sprite6;
			std::array<double,5> sprite7;
			std::array<double,5> sprite8;
			std::array<double,5> sprite9;
			std::array<double,5> fireball;
			std::array<double, 5> fireball1;
			std::array<double, 5> fireball2;
			std::array<double, 5> fireball3;

			GameObject gameObject;
			
			float alpha;

			int endX;
			int visibleTilesCount;

			int mapX;
			int mapY;
			int tileSize;
			int tileSize2d;

			//DDA
			Resources::Animation* animFurball360;
			Resources::Animation* animFireball360;
			AnimationPlayer animPlayerFur;
			AnimationPlayer animPlayerFire1;
			AnimationPlayer animPlayerFire2;
			AnimationPlayer animPlayerFire3;
			AnimationPlayer animPlayerFire4;
			AnimationPlayer animPlayer2d;

			int w;
			int h;
			double dirX, dirY; //initial direction vector
			double planeX, planeY; //the 2d raycaster version of camera plane

			int DDAX,DDAY;

			int DDAtexture;
			int drawColor;
			std::array<std::array<double,5>, 400> DDAlines; //change DDAlines size accordingly
			std::vector<std::array<double,5>> DDASpriteDrawData;
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
