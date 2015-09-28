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

		void Camera::initCamera(SDL_Window* window, float posX, float posY, float zeroToOneInPixels)
		{
			this->window = window;
			this->posX = posX;
			this->posY = posY;
			this->zeroToOneInPixels = zeroToOneInPixels;

			x_axis = glm::vec3(1.0, 0.0, 0.0);
			y_axis = glm::vec3(0.0, 1.0, 0.0);
			z_axis = glm::vec3(0.0, 0.0, 1.0);

			int width, height;
			SDL_GetWindowSize(window, &width, &height);

			this->posX = posX / (width / (this->zeroToOneInPixels * 2));

			cam_pos = glm::vec3(this->posX, this->posY, 0);
			cam_up = y_axis;
			cam_right = x_axis;
			cam_front = -z_axis;
			V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);

			
			zoomLevel = width / (this->zeroToOneInPixels);
			defaultZoomLevel = zoomLevel;
			P = glm::ortho(0.0f, zoomLevel, 0.0f, zoomLevel*height / width);
			VP = V*P;
		}

		glm::mat4 Camera::getVP()
		{
			return VP;
		}

		void Camera::setPosition(float posX, float posY)
		{
			int width, height;
			SDL_GetWindowSize(window, &width, &height);

			this->posX = posX / (width / (zeroToOneInPixels * 2));
			this->posY = posY;

			cam_pos = glm::vec3(this->posX, this->posY, 0);
			V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
			VP = V*P;
		}

		glm::vec2 Camera::getPosition()
		{
			return glm::vec2(posX, posY);
		}

		void Camera::setZoomLevel(float size)
		{
			this->zoomLevel = size;
			int width, height;
			SDL_GetWindowSize(window, &width, &height);
			P = glm::ortho(0.0f, zoomLevel, 0.0f, zoomLevel*height / width);
			VP = V*P;
		}

		void Camera::setDefaultZoomLevel()
		{
			int width, height;
			SDL_GetWindowSize(window, &width, &height);
			zoomLevel = width / (zeroToOneInPixels);
			P = glm::ortho(0.0f, zoomLevel, 0.0f, zoomLevel*height / width);
			VP = V*P;
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