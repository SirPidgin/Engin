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

		//Sprites have to have the same coordinate unit size = multiply sprite coordinates with the same number. Dont place this comment in the header...
		void Camera::initCamera(GLfloat windowWidth, GLfloat windowHeight, GLfloat posX, GLfloat posY, GLfloat coordUnitSize)
		{
			this->windowWidth = windowWidth;
			this->windowHeight = windowHeight;
			this->posX = posX;
			this->posY = posY;
			this->coordUnitSize = coordUnitSize;
			this->rotation = 0.0f;

			glm::vec3 xAxis = glm::vec3(1.0, 0.0, 0.0);
			glm::vec3 yAxis = glm::vec3(0.0, 1.0, 0.0);
			glm::vec3 zAxis = glm::vec3(0.0, 0.0, 1.0);

			this->posX = posX / (windowWidth / (this->coordUnitSize * 2));

			camPos = glm::vec3(this->posX, this->posY, 0);
			camUp = yAxis;
			camRight = xAxis;
			camFront = -zAxis;

			V = glm::lookAt(camPos, camPos + camFront, camUp);

			zoomLevel = windowWidth;
			defaultZoomLevel = zoomLevel;
			DefaultZoomOn = true;

			P = glm::ortho(0.0f, zoomLevel, 0.0f, zoomLevel*windowHeight / windowWidth);
			VP = V*P*glm::rotate(this->rotation, glm::vec3(0.0, 0.0, 1.0));
		}

		glm::mat4 Camera::getVP()
		{
			return VP;
		}

		void Camera::setPosition(GLfloat posX, GLfloat posY)
		{
			if (DefaultZoomOn)
			{
				this->posX = posX / (windowWidth / (coordUnitSize * 2));
				this->posY = posY;
			}
			else
			{
				this->posX = posX / (windowWidth / (coordUnitSize * 2))*(defaultZoomLevel / zoomLevel);
				this->posY = posY*(defaultZoomLevel / zoomLevel);
			}

			camPos = glm::vec3(this->posX, this->posY, 0);
			V = glm::lookAt(camPos, camPos + camFront, camUp);
			VP = V*P*glm::rotate(this->rotation, glm::vec3(0.0, 0.0, 1.0));
		}

		glm::vec2 Camera::getPosition()
		{
			if (DefaultZoomOn)
			{
				return glm::vec2(posX*(windowWidth / (coordUnitSize * 2)), posY);
			}
			else
			{
				return glm::vec2(posX*(windowWidth / (coordUnitSize * 2)) / (defaultZoomLevel / zoomLevel), posY / (defaultZoomLevel / zoomLevel));
			}
		}

		void Camera::setPositionCenter(GLfloat posX, GLfloat posY)
		{
			if (DefaultZoomOn)
			{
				this->posX = posX / (windowWidth / (coordUnitSize * 2)) - 1.0f;
				this->posY = posY - 1.0f;
			}
			else
			{
				this->posX = posX / (windowWidth / (coordUnitSize * 2))*(defaultZoomLevel / zoomLevel) - 1.0f;
				this->posY = posY*(defaultZoomLevel / zoomLevel) - 1.0f;
			}

			camPos = glm::vec3(this->posX, this->posY, 0);
			V = glm::lookAt(camPos, camPos + camFront, camUp);
			VP = V*P*glm::rotate(this->rotation, glm::vec3(0.0, 0.0, 1.0));
		}

		glm::vec2 Camera::getPositionCenter()
		{
			if (DefaultZoomOn)
			{
				return glm::vec2(posX*(windowWidth / (coordUnitSize * 2)) + 1.0f, posY + 1.0f);
			}
			else
			{
				return glm::vec2(posX*(windowWidth / (coordUnitSize * 2)) / (defaultZoomLevel / zoomLevel) + 1.0f, posY / (defaultZoomLevel / zoomLevel) + 1.0f);
			}
		}

		void Camera::setZoomLevel(GLfloat size)
		{
			if (size*windowWidth != zoomLevel)
			{
				this->zoomLevel = size*windowWidth;
				P = glm::ortho(0.0f, zoomLevel, 0.0f, zoomLevel*windowHeight / windowWidth);
				VP = V*P*glm::rotate(this->rotation, glm::vec3(0.0, 0.0, 1.0));
				DefaultZoomOn = false;
			}
		}

		void Camera::setDefaultZoomLevel()
		{
			zoomLevel = windowWidth;
			P = glm::ortho(0.0f, zoomLevel, 0.0f, zoomLevel*windowHeight / windowWidth);
			VP = V*P*glm::rotate(this->rotation, glm::vec3(0.0, 0.0, 1.0));
			DefaultZoomOn = true;
		}

		void Camera::setRotation(GLfloat rotation)
		{
			this->rotation = glm::radians(rotation);
			VP = VP*glm::rotate(this->rotation, glm::vec3(0.0, 0.0, 1.0));
		}

		GLfloat Camera::getRotation()
		{
			return rotation;
		}

		GLfloat Camera::getDefaultZoomLevel()
		{
			return defaultZoomLevel;
		}

		GLfloat Camera::getZoomLevel()
		{
			return zoomLevel;
		}
	}
}