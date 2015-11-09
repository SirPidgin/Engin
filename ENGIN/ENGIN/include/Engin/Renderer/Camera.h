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

			void initCamera(GLfloat viewPortX, GLfloat viewPortY, GLfloat viewPortWidth, GLfloat viewPortHeight, GLfloat worldX, GLfloat worldY, GLfloat rotationOriginX, GLfloat rotationOriginY);

			const glm::mat4& getVP() const { return VPmatrix; }
			const glm::mat4& getP() const { return P; }
			void activateViewPort();
			
			void setPositionRotationOrigin(GLfloat worldX, GLfloat worldY);
			glm::vec2 getPositionRotationOrigin();
			void setZoomLevel(GLfloat size);
			void setDefaultZoomLevel();

			void setRotation(GLfloat rotation);
			//Place this at the end of update to fix coordinates because camera rotates from the corner.
			void fixCoordinatesForRotationAtTheEndOfUpdate();

			GLfloat getRotation();
			GLfloat getZoomLevel();
			void setNewRotationOrigin(GLfloat worldX, GLfloat worldY);
			glm::vec2 getRotationOrigin();

		private:
			GLfloat zoomLevel;

			GLfloat worldX;
			GLfloat worldY;
			GLfloat rotation;
			GLfloat coordMultip;
			GLfloat getMethodCoordMultip;
			GLfloat rotationOriginX, rotationOriginY;

			GLfloat viewPortWidth, viewPortHeight;

			glm::mat4 P;
			glm::mat4 VPmatrix; //getVP

			glm::mat4 scaleMatrix;
			glm::mat4 positionMatrix;
			glm::mat4 rotationMatrix;

			glm::vec4 viewPort;

			GLfloat root;
			GLfloat atani;
		};
	}
}