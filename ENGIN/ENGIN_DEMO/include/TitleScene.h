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
			Resources::Texture* titleTexture;
			Resources::ShaderProgram* textureShader;
			Renderer::Camera* camera;

			// Resources. Used in Pseudo 3D.
			void loadResources();

			Resources::ShaderProgram* shader;
			Resources::ShaderProgram* alphaShader;
			Resources::Texture* furball;
			Resources::Texture* furball_128;
			Resources::Texture* tree_64;
			Resources::Texture* mapSheet_64;
			Resources::Texture* mapSheet_256;
			Resources::Texture* floor_16;
			Resources::Texture* floor_800;
			Resources::Font* font;
			Resources::Animation* animFurball360;
			Resources::Texture* furballShadow;
			Resources::Animation* animFireball360;
			Resources::Animation* animTree360;
			Resources::Texture* treeShadow;
			Resources::Animation* animFurballHit;

			Engin* engine;
		};
	}
}