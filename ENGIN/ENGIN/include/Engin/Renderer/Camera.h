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

			const glm::mat4& getVP() const { return VP/**positionMatrix*rotationMatrix*scaleMatrix*/; }
			const glm::mat4& getP() const { return P; }
			void activateViewPort();
			void setPosition(GLfloat worldX, GLfloat worldY);
			glm::vec2 getPosition();
			void setPositionCenter(GLfloat worldX, GLfloat worldY);
			glm::vec2 getPositionCenter();
			void setZoomLevel(GLfloat size);
			void setDefaultZoomLevel();
			void setRotation(GLfloat rotation);
			GLfloat getRotation();
			/*GLfloat getZoomLevel();
			GLfloat getDefaultZoomLevel();*/

		private:
			/*GLfloat zoomLevel;
			GLfloat defaultZoomLevel;*/
			GLfloat coordUnitSize;
			GLfloat worldX;
			GLfloat worldY;
			GLfloat rotation;
			GLfloat coordMultipX;
			GLfloat coordMultipY;
			GLfloat getMethodCoordMultipX;
			GLfloat getMethodCoordMultipY;

			GLfloat viewPortWidth, viewPortHeight;

			glm::vec3 camPos;
			glm::vec3 camUp;
			glm::vec3 camRight;
			glm::vec3 camFront;

			glm::mat4 V;
			glm::mat4 P;
			glm::mat4 VP;

			glm::mat4 scaleMatrix;
			glm::mat4 positionMatrix;
			glm::mat4 rotationMatrix;

			glm::vec4 viewPort;
		};
	}
}