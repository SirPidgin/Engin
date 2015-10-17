#pragma once

#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "Camera.h"
#include "Color.h"
#include "Engin\Resources\Shader.h"

namespace Engin
{
	namespace Renderer
	{
		struct Vertex
		{
			glm::vec3 position;
			glm::vec4 color;

			Vertex() : position(0.0f), color(0.0f) {}
			Vertex(const glm::vec3& position, const glm::vec4& color) : position(position), color(color) {}
			Vertex(Vertex&& other) { position = std::move(other.position); color = std::move(other.color); }
		};

		// TODO (eeneku): Batch should also make use of indices

		class Batch
		{
		public:
			Batch(){};
			//Batch(Resources::Shader* shader, size_t maxVertices = defaultBatchSize);
			~Batch();

			void addShader(Resources::Shader* shader, size_t maxVertices = defaultBatchSize);

			void draw(const std::vector<Vertex>& vertices);
			void drawTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, const Color& color, GLfloat depth = 0);
			void drawQuad(GLfloat x, GLfloat y, GLfloat width, GLfloat height, const Color& color, GLfloat depth = 0);
			void flush(const Camera& camera);
			void clear() { currentVertex = 0; }

		private:
			std::vector<Vertex> vertices;

			static const size_t defaultBatchSize = 2048;

			GLuint VBO;
			GLuint currentVertex;

			Resources::Shader* shader;
		};
	}
}
