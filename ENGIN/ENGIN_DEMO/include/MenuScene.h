#include "Engin\Engin.h"
#include "Engin\Game\Scene.h"

#include "Engin\Resources\Font.h"
#include "Engin\Resources\ShaderProgram.h"
#include "Engin\Resources\Texture.h"

#include "Engin\Renderer\TextRenderer.h"

namespace Engin
{
	namespace Game
	{
		class MenuScene : public Scene
		{
		public:
			MenuScene(Engin* engine);
			~MenuScene();

			void update(GLfloat step);
			void interpolate(GLfloat alpha);
			void draw();

		private:
			enum Option
			{
				FURBALLSTEIN = 0,
				ORIGINAL,
				QUIT
			};

			Option selected;

			Engin* engine;

			Resources::Texture* menuTexture;
			Resources::ShaderProgram* shader;
			Resources::Font* font;

			Resources::Texture* optionFurballstein;
			Resources::Texture* optionOriginal;
			Resources::Texture* optionQuit;

			Renderer::TextRenderer optionFurballsteinCreator;
			Renderer::TextRenderer optionOriginalCreator;
			Renderer::TextRenderer optionQuitCreator;

			Renderer::Camera* camera;
		};
	}
}