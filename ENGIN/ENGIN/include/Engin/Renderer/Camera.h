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

			void initCamera(GLfloat viewPortX, GLfloat viewPortY, GLfloat viewPortWidth, GLfloat viewPortHeight, GLfloat worldX, GLfloat worldY, GLfloat coordUnitSize);

			const glm::mat4& getVP() const { return VP; };
			void activateViewPort();
			void setPosition(GLfloat worldX, GLfloat worldY);
			glm::vec2 getPosition();
			void setPositionCenter(GLfloat worldX, GLfloat worldY);
			glm::vec2 getPositionCenter();
			void setZoomLevel(GLfloat size);
			void setDefaultZoomLevel();
			void rotate(GLfloat rotation);
			GLfloat getRotation();
			GLfloat getZoomLevel();
			GLfloat getDefaultZoomLevel();

		private:
			GLfloat zoomLevel;
			GLfloat defaultZoomLevel;
			GLfloat coordUnitSize;
			GLfloat worldX;
			GLfloat worldY;
			GLfloat rotation;
			GLfloat coordMultipX;
			GLfloat coordMultipY;
			GLfloat getMethodCoordMultipX;
			GLfloat getMethodCoordMultipY;

			GLfloat zoomCoordMultipMultip;

			GLfloat viewPortWidth, viewPortHeight;

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