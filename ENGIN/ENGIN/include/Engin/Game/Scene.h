#pragma once
#include <GL\glew.h>
#include <vector>
#include "Engin\Renderer\Camera.h"
#include "Engin\Renderer\TextureBatch.h"

namespace Engin
{
	namespace Game
	{
		class Scene
		{
		public:
			Scene();
			virtual ~Scene(){};
			
			// Scene update.
			virtual void update(GLfloat step) = 0;

			// Scene interpolate.
			virtual void interpolate(GLfloat alpha) = 0;

			// Scene draw.
			virtual void draw() = 0;
			
			void flushBatches();
			void begin();
			void end();
		
		protected:
			Renderer::Camera* createWorldCamera();
			Renderer::Camera* createGuiCamera();

			Renderer::TextureBatch opaqueBatch;
			Renderer::TextureBatch alphaBatch;
			Renderer::TextureBatch guiBatch;
	
		private:
			std::vector<Renderer::Camera*> worldCameras;
			std::vector<Renderer::Camera*> guiCameras;
		};
	}
}