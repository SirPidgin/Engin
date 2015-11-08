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

		void Camera::initCamera(GLfloat viewPortX, GLfloat viewPortY, GLfloat viewPortWidth, GLfloat viewPortHeight, GLfloat worldX, GLfloat worldY, GLfloat coordUnitSize, GLfloat rotationOriginX, GLfloat rotationOriginY)
		{
			this->viewPortWidth = viewPortWidth;
			this->viewPortHeight = viewPortHeight;
			this->worldX = worldX;
			this->worldY = worldY;
			this->coordUnitSize = coordUnitSize;
			this->rotation = 0.0f;
			this->zoomLevel = 1.0f;

			this->rotationOriginX = rotationOriginX;
			this->rotationOriginY = rotationOriginY;

			viewPort = glm::vec4(viewPortX, viewPortY, viewPortWidth, viewPortHeight);

			this->worldX = this->worldX * coordMultip;
			this->worldY = this->worldY * coordMultip;
						
			setRotation(this->rotation);
			setPosition(this->worldX, this->worldY);
			setDefaultZoomLevel();

			P = glm::ortho(0.0f, viewPortWidth, 0.0f, viewPortHeight);
		}

		void Camera::activateViewPort()
		{
			fixCoordinatesForRotationAtTheEndOfUpdate(); //Get rid of this? Or move to update.
			VPmatrix = P*positionMatrix*rotationMatrix*scaleMatrix; //Temporal location for calculating VP matrix.
			glViewport(viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
		}

		void Camera::setPosition(GLfloat worldX, GLfloat worldY) //Not working with rotate anymore. Remove?
		{
			this->worldX = worldX*coordMultip;
			this->worldY = worldY*coordMultip;

			positionMatrix = glm::translate(glm::vec3(-this->worldX, -this->worldY, 0.0f)); //Camera translate is to an opposite direction
		}

		glm::vec2 Camera::getPosition()
		{
			return glm::vec2(worldX, worldY*getMethodCoordMultip);
		}

		void Camera::setPositionCenter(GLfloat worldX, GLfloat worldY) //Not working with rotate anymore. Remove?
		{
			this->worldX = worldX*coordMultip - 0.5f*viewPortWidth;
			this->worldY = worldY*coordMultip - 0.5f*viewPortHeight;

			positionMatrix = glm::translate(glm::vec3(-this->worldX, -this->worldY, 0.0f)); //Camera translate is to an opposite direction
		}

		glm::vec2 Camera::getPositionCenter()
		{
			return glm::vec2((worldX + 0.5f*viewPortWidth), (worldY + 0.5f*viewPortHeight)*getMethodCoordMultip);
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
				coordMultip = size*this->coordUnitSize;
				getMethodCoordMultip = 1.0f / size;
				scaleMatrix = glm::scale(glm::vec3(size, size, 1.0f));
				setPositionRotationOrigin(tempWorldX, tempWorldY);
			}
			if (size <= 0.0f)//if zoom is 0 you cant see how it moves. negative zoom neglected
			{
				zoomLevel = 0.0f;
				coordMultip = this->coordUnitSize;
				getMethodCoordMultip = 1.0f;
				scaleMatrix = glm::scale(glm::vec3(0.0f, 0.0f, 1.0f));
				setPositionRotationOrigin(tempWorldX, tempWorldY);
			}			
		}

		void Camera::setDefaultZoomLevel()
		{
			coordMultip = this->coordUnitSize;
			getMethodCoordMultip = 1.0f;
			scaleMatrix = glm::scale(glm::vec3(1.0f));
		}

		void Camera::setRotation(GLfloat rotation)
		{
			this->rotation = glm::radians(-rotation); //Camera rotates to the opposite direction.
			rotationMatrix = glm::rotate(this->rotation, glm::vec3(0.0f, 0.0f, 1.0f));			
		}

		void Camera::fixCoordinatesForRotationAtTheEndOfUpdate()
		{
			if (rotation != 0.0f)
			{
				GLfloat tempX;
				GLfloat tempY;

				root = glm::sqrt(glm::pow(worldX + rotationOriginX, 2.0f) + glm::pow(worldY + rotationOriginY, 2.0f));
				atani = glm::atan(worldY + rotationOriginY, worldX + rotationOriginX);

				tempX = (root * cos(this->rotation + atani) - rotationOriginX);
				tempY = (root * sin(this->rotation + atani) - rotationOriginY);

				//root = glm::sqrt(glm::pow(worldX + 0.5f*viewPortWidth, 2.0f) + glm::pow(worldY + 0.5f*viewPortHeight, 2.0f));
				//atani = glm::atan(worldY + 0.5f*viewPortHeight, worldX + 0.5f*viewPortWidth);

				//tempX = (root * cos(this->rotation + atani) - 0.5f*viewPortWidth);
				//tempY = (root * sin(this->rotation + atani) - 0.5f*viewPortHeight);
				positionMatrix = glm::translate(glm::vec3(-tempX, -tempY, 0.0f));
			}			
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