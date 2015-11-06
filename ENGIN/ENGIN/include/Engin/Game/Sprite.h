#pragma once

#include <vector>

#include "Component.h"
#include "Engin\Resources\Texture.h"
#include "Engin\Renderer\TextureBatch.h"


namespace Engin
{
	namespace Game
	{
		class Sprite : public Component
		{
		public:
			Sprite(Renderer::TextureBatch* textureBatch);
			~Sprite();

			void draw();

			void setCurrentSprite(Resources::Texture* texture);
			//Animations?

		private:
			Renderer::TextureBatch* textureBatch;
			Resources::Texture* currentTexture;
		};
	}
}