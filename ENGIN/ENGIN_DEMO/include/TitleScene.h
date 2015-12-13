#include "Engin\Engin.h"
#include "Engin\Game\Scene.h"
#include "Engin\Resources\Animation.h"
#include "Engin\Resources\Font.h"
#include "Engin\Resources\ShaderProgram.h"
#include "Engin\Resources\Texture.h"

namespace Engin
{
	namespace Game
	{
		class TitleScene : public Scene
		{
		public:
			TitleScene(Engin* engine);
			~TitleScene();

			void update(GLfloat step);
			void interpolate(GLfloat alpha);
			void draw();

		private:
			
			
			Renderer::Camera* camera;

			// Resources.
			void loadResources();

			Resources::ShaderProgram* shader;
			Resources::ShaderProgram* alphaShader;
			Resources::ShaderProgram* textureShader;
			Resources::Texture* titleTexture;
			Resources::Texture* furball;
			Resources::Texture* furball_128;
			Resources::Texture* tree_64;
			Resources::Texture* mapSheet_64;
			Resources::Texture* mapSheet_256;
			Resources::Texture* floor_16;
			Resources::Texture* floor_800;
			Resources::Texture* treeShadow;
			Resources::Texture* furballShadow;
			Resources::Texture* doge;
			Resources::Font* font;
			Resources::Animation* animFurball360;
			Resources::Animation* animFireball360;
			Resources::Animation* animTree360;
			Resources::Animation* animFurballHit;
			Resources::Animation* explosionAnimation1;
			Resources::Animation* explosionAnimation2;

			Engin* engine;
		};
	}
}