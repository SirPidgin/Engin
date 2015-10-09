#include "Engin\Renderer\Camera.h"

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

		void Camera::initCamera(float windowWidth, float windowHeight, float posX, float posY, float zeroToOneInPixels)
		{
			this->windowWidth = windowWidth;
			this->windowHeight = windowHeight;
			this->posX = posX;
			this->posY = posY;
			this->zeroToOneInPixels = zeroToOneInPixels;

			x_axis = glm::vec3(1.0, 0.0, 0.0);
			y_axis = glm::vec3(0.0, 1.0, 0.0);
			z_axis = glm::vec3(0.0, 0.0, 1.0);

			this->posX = posX / (windowWidth / (this->zeroToOneInPixels * 2));

			cam_pos = glm::vec3(this->posX, this->posY, 0);
			cam_up = y_axis;
			cam_right = x_axis;
			cam_front = -z_axis;
			V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
			
			zoomLevel = windowWidth / (this->zeroToOneInPixels);
			defaultZoomLevel = zoomLevel;
			DefaultZoomOn = true;

			P = glm::ortho(0.0f, zoomLevel, 0.0f, zoomLevel*windowHeight / windowWidth);
			VP = V*P;
		}

		glm::mat4 Camera::getVP()
		{
			return VP;
		}

		void Camera::setPosition(float posX, float posY)
		{
			if (DefaultZoomOn)
			{
				this->posX = posX / (windowWidth / (zeroToOneInPixels * 2));
				this->posY = posY;
			}
			else //if not default zoom
			{
				this->posX = posX / (windowWidth / (zeroToOneInPixels * 2))*(defaultZoomLevel / zoomLevel);
				this->posY = posY*(defaultZoomLevel / zoomLevel);
			}

			cam_pos = glm::vec3(this->posX, this->posY, 0);
			V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
			VP = V*P;
		}

		glm::vec2 Camera::getPosition()
		{
			if (DefaultZoomOn)
			{
				return glm::vec2(posX*(windowWidth / (zeroToOneInPixels * 2)), posY);
			}
			else //if not default
			{
				return glm::vec2(posX*(windowWidth / (zeroToOneInPixels * 2)) / (defaultZoomLevel / zoomLevel), posY / (defaultZoomLevel / zoomLevel));
			}
		}

		void Camera::setZoomLevel(float size)
		{
			this->zoomLevel = size;
			P = glm::ortho(0.0f, zoomLevel, 0.0f, zoomLevel*windowHeight / windowWidth);
			VP = V*P;
			DefaultZoomOn = false;
		}

		void Camera::setDefaultZoomLevel()
		{
			zoomLevel = windowWidth / (zeroToOneInPixels);
			P = glm::ortho(0.0f, zoomLevel, 0.0f, zoomLevel*windowHeight / windowWidth);
			VP = V*P;
			DefaultZoomOn = true;
		}

		float Camera::getDefaultZoomLevel()
		{
			return defaultZoomLevel;
		}

		float Camera::getZoomLevel()
		{
			return zoomLevel;
		}
	}
}