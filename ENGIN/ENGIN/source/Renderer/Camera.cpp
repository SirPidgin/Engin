#include "Engin\Renderer\Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace Engin
{
	namespace Renderer
	{
		Camera::Camera()
		{
		}

		Camera::~Camera()
		{
		}

		void Camera::initCamera(GLfloat viewPortX, GLfloat viewPortY, GLfloat viewPortWidth, GLfloat viewPortHeight, GLfloat worldX, GLfloat worldY, GLfloat rotationOriginX, GLfloat rotationOriginY)
		{
			this->viewPortWidth = viewPortWidth;
			this->viewPortHeight = viewPortHeight;
			this->worldX = worldX;
			this->worldY = worldY;
			this->rotation = 0.0f;

			this->rotationOriginX = rotationOriginX;
			this->rotationOriginY = rotationOriginY;

			viewPort = glm::vec4(viewPortX, viewPortY, viewPortWidth, viewPortHeight);						
			setDefaultZoomLevel();

			this->worldX = this->worldX*coordMultip;
			this->worldY = this->worldY*coordMultip;

			setPositionRotationOrigin(this->worldX, this->worldY);			

			P = glm::ortho(0.0f, viewPortWidth, 0.0f, viewPortHeight);
		}

		void Camera::activateViewPort()
		{
			calculateVP();
			glViewport(viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
		}

		void Camera::setPositionRotationOrigin(GLfloat worldX, GLfloat worldY)
		{
			this->worldX = worldX*coordMultip - rotationOriginX;
			this->worldY = worldY*coordMultip - rotationOriginY;

			positionMatrix = glm::translate(glm::vec3(-this->worldX, -this->worldY, 0.0f)); //Camera translate is to an opposite direction
		}

		glm::vec2 Camera::getPositionRotationOrigin()
		{
			return glm::vec2((worldX + rotationOriginX), (worldY + rotationOriginY))*getMethodCoordMultip;
		}

		void Camera::setZoomLevel(GLfloat size) //Test this more.
		{
			GLfloat tempWorldX = getPositionRotationOrigin().x;
			GLfloat tempWorldY = getPositionRotationOrigin().y;

			if (size > 0.0001f)
			{
				zoomLevel = size;
				coordMultip = size;
				getMethodCoordMultip = 1.0f/zoomLevel;
				scaleMatrix = glm::scale(glm::vec3(size, size, 1.0f));
				setPositionRotationOrigin(tempWorldX, tempWorldY);
			}
			if (size <= 0.0f)//if zoom is 0 you cant see how it moves. negative zoom neglected
			{
				zoomLevel = 0.0f;
				coordMultip = 1.0f;
				getMethodCoordMultip = 1.0f;
				scaleMatrix = glm::scale(glm::vec3(0.0f, 0.0f, 1.0f));
				setPositionRotationOrigin(tempWorldX, tempWorldY);
			}			
		}

		void Camera::setDefaultZoomLevel()
		{
			zoomLevel = 1.0f;
			coordMultip = 1.0f;
			getMethodCoordMultip = 1.0f;
			scaleMatrix = glm::scale(glm::vec3(1.0f));
		}

		void Camera::setRotation(GLfloat rotation)
		{
			this->rotation = glm::radians(-rotation); //Camera rotates to the opposite direction.
			rotationMatrix = glm::rotate(this->rotation, glm::vec3(0.0f, 0.0f, 1.0f));			
		}

		void Camera::calculateVP()
		{
			if (rotation != 0.0f)
			{
				root = glm::sqrt(glm::pow(worldX + rotationOriginX, 2.0f) + glm::pow(worldY + rotationOriginY, 2.0f));
				atani = glm::atan(worldY + rotationOriginY, worldX + rotationOriginX);

				tempX = (root * cos(this->rotation + atani) - rotationOriginX);
				tempY = (root * sin(this->rotation + atani) - rotationOriginY);

				positionMatrix = glm::translate(glm::vec3(-tempX, -tempY, 0.0f));
			}			
			VPmatrix = P*positionMatrix*rotationMatrix*scaleMatrix;
		}

		GLfloat Camera::getRotation()
		{
			return glm::degrees(-this->rotation);
		}

		GLfloat Camera::getZoomLevel()
		{
			return zoomLevel;
		}

		void Camera::setNewRotationOrigin(GLfloat worldX, GLfloat worldY)
		{
			rotationOriginX = worldX;
			rotationOriginY = worldY;
		}

		glm::vec2 Camera::getRotationOrigin()
		{
			return glm::vec2(rotationOriginX, rotationOriginY);
		}
	}
}