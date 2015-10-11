#pragma once

#include <GL\glew.h>
#include <glm/glm.hpp>

namespace Engin
{
	namespace Renderer
	{
		class Camera
		{
		public:
			Camera();
			~Camera();

			void initCamera(GLfloat windowWidth, GLfloat windowHeight, GLfloat posX, GLfloat posY, GLfloat coordUnitSize, GLfloat viewPortX, GLfloat viewPortY, GLfloat viewPortWidth, GLfloat viewPortHeight);

			glm::mat4 getVP();
			void setPosition(GLfloat posX, GLfloat posY);
			glm::vec2 getPosition();
			void setPositionCenter(GLfloat posX, GLfloat posY);
			glm::vec2 getPositionCenter();
			void setZoomLevel(GLfloat size);
			void setDefaultZoomLevel();
			void setRotation(GLfloat rotation);
			GLfloat getRotation();
			GLfloat getZoomLevel();
			GLfloat getDefaultZoomLevel();

		private:
			GLfloat zoomLevel;
			GLfloat defaultZoomLevel;
			GLfloat coordUnitSize;
			GLfloat posX;
			GLfloat posY;
			GLfloat rotation;
			bool DefaultZoomOn;

			GLfloat windowWidth, windowHeight;

			glm::vec3 camPos;
			glm::vec3 camUp;
			glm::vec3 camRight;
			glm::vec3 camFront;

			glm::mat4 V;
			glm::mat4 P;
			glm::mat4 VP;

			glm::vec4 viewPort;
		};
	}
}