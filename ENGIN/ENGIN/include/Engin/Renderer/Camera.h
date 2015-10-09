#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL/SDL.h>

namespace Engin
{
	namespace Renderer
	{
		class Camera
		{
		public:
			Camera();
			~Camera();

			void initCamera(float windowWidth, float windowHeight, float posX, float posY, float zeroToOneInPixels);

			glm::mat4 getVP();
			void setPosition(float posX, float posY);
			glm::vec2 getPosition();
			void setZoomLevel(float size);
			void setDefaultZoomLevel();
			float getZoomLevel();
			float getDefaultZoomLevel();

		private:
			float zoomLevel;
			float defaultZoomLevel;
			float zeroToOneInPixels;
			float posX;
			float posY;
			bool DefaultZoomOn;

			float windowWidth, windowHeight;

			glm::vec3 x_axis;
			glm::vec3 y_axis;
			glm::vec3 z_axis;

			glm::vec3 cam_pos;
			glm::vec3 cam_up;
			glm::vec3 cam_right;
			glm::vec3 cam_front;

			glm::mat4 V;
			glm::mat4 P;
			glm::mat4 VP;
		};
	}
}