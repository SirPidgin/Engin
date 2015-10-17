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
			this->zoomLevel = 1.0f;

			viewPort = glm::vec4(viewPortX, viewPortY, viewPortWidth, viewPortHeight);

			glm::vec3 xAxis = glm::vec3(1.0, 0.0, 0.0);
			glm::vec3 yAxis = glm::vec3(0.0, 1.0, 0.0);
			glm::vec3 zAxis = glm::vec3(0.0, 0.0, 1.0);

			coordMultipX = this->coordUnitSize;
			coordMultipY = this->coordUnitSize;

			getMethodCoordMultipX = 1 / this->coordUnitSize;
			getMethodCoordMultipY = 1 / this->coordUnitSize;

			this->worldX = this->worldX * coordMultipX;
			this->worldY = this->worldY * coordMultipY;

			camPos = glm::vec3(0.0f, 0.0f, 0.0f);
			camUp = yAxis;
			camRight = xAxis;
			camFront = -zAxis;

			V = glm::lookAt(camPos, camPos + camFront, camUp);
						
			setRotation(this->rotation);
			setPosition(this->worldX, this->worldY);
			setDefaultZoomLevel();

			P = glm::ortho(0.0f, viewPortWidth, 0.0f, viewPortHeight);
			VP = V*P;
		}

		void Camera::activateViewPort()
		{
			VPmatrix = VP*positionMatrix*rotationMatrix*scaleMatrix; //Temporal location for calculating VP matrix.
			glViewport(viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
		}

		void Camera::setPosition(GLfloat worldX, GLfloat worldY)
		{
			this->worldX = worldX*coordMultipX;
			this->worldY = worldY*coordMultipY;

			positionMatrix = glm::translate(glm::vec3(this->worldX*-1, this->worldY*-1, 0.0f)); //Camera translate is to an opposite direction
		}

		glm::vec2 Camera::getPosition()
		{
			return glm::vec2(worldX*getMethodCoordMultipX, worldY*getMethodCoordMultipY);
		}

		void Camera::setPositionCenter(GLfloat worldX, GLfloat worldY)
		{
			this->worldX = (worldX - 0.5f*viewPortWidth)*coordMultipX;
			this->worldY = (worldY - 0.5f*viewPortHeight)*coordMultipY;

			positionMatrix = glm::translate(glm::vec3(this->worldX*-1, this->worldY*-1, 0.0f)); //Camera translate is to an opposite direction
		}

		glm::vec2 Camera::getPositionCenter()
		{
			return glm::vec2(worldX*getMethodCoordMultipX + 0.5f*viewPortWidth, worldY*getMethodCoordMultipY + 0.5f*viewPortHeight);
		}

		void Camera::setZoomLevel(GLfloat size) //Test this more.
		{
			zoomLevel = size;
			coordMultipX = 1.0f / (coordMultipX / (coordMultipX*size)); 
			coordMultipY = 1.0f / (coordMultipY / (coordMultipY*size));

			getMethodCoordMultipX = 1 / coordMultipX;
			getMethodCoordMultipY = 1 / coordMultipY;

			scaleMatrix = glm::scale(glm::vec3(size));
		}

		void Camera::setDefaultZoomLevel()
		{
			scaleMatrix = glm::scale(glm::vec3(1.0f));
		}

		void Camera::setRotation(GLfloat rotation)
		{
			this->rotation = glm::radians(-rotation); //Cameras rotate to the opposite direction.

			{
				GLfloat tempX;
				GLfloat tempY;

				root = glm::sqrt(glm::pow(getPositionCenter().x, 2.0f) + glm::pow(getPositionCenter().y, 2.0f));
				atani = glm::atan(getPositionCenter().y, getPositionCenter().x);

				tempX = (root * cos(this->rotation + atani) - 0.5f*viewPortWidth)*coordMultipX;
				tempY = (root * sin(this->rotation + atani) - 0.5f*viewPortHeight)*coordMultipY;
				positionMatrix = glm::translate(glm::vec3(tempX*-1, tempY*-1, 0.0f));

				//setPositionCenter(tempX + root * cos(this->rotation + atani), tempY + root*sin(this->rotation + atani)); //Old code delete comment
				rotationMatrix = glm::rotate(this->rotation, glm::vec3(0.0f, 0.0f, 1.0f));		
			}			
		}

		GLfloat Camera::getRotation()
		{
			return rotation;
		}

		GLfloat Camera::getZoomLevel()
		{
			return zoomLevel;
		}
	}
}