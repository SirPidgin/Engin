#pragma once

#include <Engin\Engin.h>
#include "Engin\Game\Scene.h"
#include <Engin\Renderer\Camera.h>
#include <Engin\Renderer\Batch.h>
#include <Engin\Renderer\TextureBatch.h>
#include "Engin\Resources\ResourceManager.h"
#include "Engin\Resources\Texture.h"
#include "Engin\Resources\Font.h"
#include "Engin\Renderer\TextRenderer.h"
#include "Engin\Resources\ShaderProgram.h"
#include "Engin\Resources\Animation.h"
#include "Engin\Game\AnimationPlayer.h"
#include "Engin\Game\ParticleEffect.h"

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
			void renderTilemap(float startX, float startY, float tileWidth, float tileHeight, size_t worldWidth, size_t worldHeight, Renderer::Batch& batch);
			void renderDogemap(float startX, float startY, float tileWidth, float tileHeight, size_t worldWidth, size_t worldHeight);
			void renderTexture(Resources::Texture* texture, float x, float y, const Renderer::Camera& camera);
		private:
			GLfloat alpha;

			Engin* engine;

			Renderer::Camera* camera;
			Renderer::Camera* camera1;
			Renderer::Camera* camera2;

			Resources::ShaderProgram* shader;
			Resources::ShaderProgram* textureShader;
			Resources::ShaderProgram* alphaShader;

			Resources::Texture* doge;

			std::string textString;
			Resources::Font* font;
			Renderer::TextRenderer* textCreator; //Change TextRenderer class name
			Resources::Texture* textTexture;	
			Resources::Animation* animation;
			Resources::Animation* animation1;

			Game::AnimationPlayer animationPlayer;
			Game::AnimationPlayer animationPlayer1;

			bool useGamePad;
		};
	}
}
