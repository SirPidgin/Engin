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

		void Camera::initCamera(GLfloat viewPortX, GLfloat viewPortY, GLfloat viewPortWidth, GLfloat viewPortHeight, GLfloat worldX, GLfloat worldY, GLfloat coordUnitSize)
		{
			this->viewPortWidth = viewPortWidth;
			this->viewPortHeight = viewPortHeight;
			this->worldX = worldX;
			this->worldY = worldY;
			this->coordUnitSize = coordUnitSize;
			this->rotation = 0.0f;

			viewPort = glm::vec4(viewPortX, viewPortY, viewPortWidth, viewPortHeight);

			glm::vec3 xAxis = glm::vec3(1.0, 0.0, 0.0);
			glm::vec3 yAxis = glm::vec3(0.0, 1.0, 0.0);
			glm::vec3 zAxis = glm::vec3(0.0, 0.0, 1.0);

			coordMultipX = 1 / (this->viewPortWidth / (this->coordUnitSize * 2));
			coordMultipY = 1 / (this->viewPortHeight / (this->coordUnitSize * 2));

			getMethodCoordMultipX = (this->viewPortWidth / (this->coordUnitSize * 2));
			getMethodCoordMultipY = (this->viewPortHeight / (this->coordUnitSize * 2));

			this->worldX = worldX * coordMultipX;
			this->worldY = worldY * coordMultipY;

			camPos = glm::vec3(this->worldX, this->worldY, 0);
			camUp = yAxis;
			camRight = xAxis;
			camFront = -zAxis;

			V = glm::lookAt(camPos, camPos + camFront, camUp);

			/*zoomLevel = viewPortWidth;
			defaultZoomLevel = zoomLevel;*/

			setPosition(this->worldX, this->worldY);
			setRotation(this->rotation);
			setDefaultZoomLevel();

			P = glm::ortho(0.0f, viewPortWidth, 0.0f, /*zoomLevel**/viewPortHeight /*/ viewPortWidth*/);
			VP = V*P/**glm::rotate(this->rotation, glm::vec3(0.0, 0.0, 1.0))*/;
		}

		void Camera::activateViewPort()
		{
			glViewport(viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
		}

		void Camera::setPosition(GLfloat worldX, GLfloat worldY)
		{
			this->worldX = worldX*coordMultipX;
			this->worldY = worldY*coordMultipY;

			positionMatrix = glm::translate(glm::vec3(this->worldX, this->worldY, 0.0f));

			/*camPos = glm::vec3(this->worldX, this->worldY, 0);
			V = glm::lookAt(camPos, camPos + camFront, camUp);*/
			//VP = VP*glm::translate(glm::vec3(this->worldX, this->worldY, 0.0f));
		}

		glm::vec2 Camera::getPosition()
		{
			return glm::vec2(worldX*getMethodCoordMultipX, worldY*getMethodCoordMultipY);
		}

		void Camera::setPositionCenter(GLfloat worldX, GLfloat worldY)
		{
			/*if (DefaultZoomOn)
			{
			this->worldX = worldX / (viewPortWidth / (coordUnitSize * 2)) - 1.0f;
			this->worldY = worldY - 1.0f;
			}
			else
			{
			this->worldX = worldX / (viewPortWidth / (coordUnitSize * 2))*(defaultZoomLevel / zoomLevel) - 1.0f;
			this->worldY = worldY*(defaultZoomLevel / zoomLevel) - 1.0f;
			}*/

			this->worldX = worldX * coordMultipX - 1.0f;
			this->worldY = worldY * coordMultipY - 1.0f;

			/*camPos = glm::vec3(this->worldX, this->worldY, 0);
			V = glm::lookAt(camPos, camPos + camFront, camUp);*/

			positionMatrix = glm::translate(glm::vec3(this->worldX - 1.0f, this->worldY - 1.0f, 0.0f));

			//VP = VP*glm::translate(glm::vec3(this->worldX-1.0f, this->worldY - 1.0f, 0.0f));
		}

		glm::vec2 Camera::getPositionCenter()
		{
			return glm::vec2(worldX*getMethodCoordMultipX + 1.0f, worldY*getMethodCoordMultipY + 1.0f);
		}

		void Camera::setZoomLevel(GLfloat size)
		{
			//this->zoomLevel = size*viewPortWidth;

			coordMultipX = 1 / (viewPortWidth / (coordUnitSize * 2))/**(defaultZoomLevel / zoomLevel)*/;
			//coordMultipY = (defaultZoomLevel / zoomLevel);

			getMethodCoordMultipX = 1 / coordMultipX /*/ (defaultZoomLevel / zoomLevel)*/;
			getMethodCoordMultipY = 1 / coordMultipY /*/ (defaultZoomLevel / zoomLevel)*/;

			scaleMatrix = glm::scale(glm::vec3(size));

			//P = glm::ortho(0.0f, zoomLevel, 0.0f, zoomLevel*viewPortHeight / viewPortWidth);
			//VP = V*P*glm::rotate(this->rotation, glm::vec3(0.0, 0.0, 1.0));
			//VP = VP*glm::scale(glm::vec3(size));
		}

		void Camera::setDefaultZoomLevel()
		{
			scaleMatrix = glm::scale(glm::vec3(1.0f));
		}

		void Camera::setRotation(GLfloat rotation)
		{
			this->rotation = glm::radians(rotation);
			rotationMatrix = glm::rotate(this->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			//VP = VP*glm::rotate(this->rotation, glm::vec3(0.0, 0.0, 1.0));
		}

		GLfloat Camera::getRotation()
		{
			return rotation;
		}

		/*GLfloat Camera::getDefaultZoomLevel()
		{
			return defaultZoomLevel;
		}

		GLfloat Camera::getZoomLevel()
		{
			return zoomLevel;
		}*/
	}
}