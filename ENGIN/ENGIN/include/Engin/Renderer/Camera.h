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

			// Location in windowX, Location in windowY, width and height of the viewport, world x and y position, origin x and y relative to camera. (0,0) is left down corner, viewport resolution is the up right corner.
			void initCamera(GLfloat viewPortX, GLfloat viewPortY, GLfloat viewPortWidth, GLfloat viewPortHeight, GLfloat worldX, GLfloat worldY, GLfloat rotationOriginX, GLfloat rotationOriginY);

			// Returns mat4 view projection matrix from the camera.
			const glm::mat4& getVP() const { return VPmatrix; }

			// Returns mat4 projection matrix from the camera.
			const glm::mat4& getP() const { return P; }

			// Actives the viewport for the camera. This method also runs the calculateVP.
			void activateViewPort();
			
			// Set camera position. Camera origin is the control point.
			void setPositionRotationOrigin(GLfloat worldX, GLfloat worldY);

			// Returns the camera position relative to the world from the origin of the camera.
			glm::vec2 getPositionRotationOrigin();

			// Camera zoom is relative to the camera origin. 0 is infinite far. There is no maximum zoom limit for getting closer.
			void setZoomLevel(GLfloat size);
			void setDefaultZoomLevel();

			void setRotation(GLfloat rotation);
			// Place this at the end of update to fix coordinates because camera rotates from the corner.
			void calculateVP();

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

			GLfloat tempX;
			GLfloat tempY;
		};
	}
}