#include "Engin\Renderer\Batch.h"

#include <cassert>
#include <glm\gtc\type_ptr.hpp>

namespace Engin
{
	namespace Renderer
	{
		Batch::Batch(Resources::Shader* shader, size_t maxVertices) : currentVertex(0), shader(shader)
		{
			assert(maxVertices > 0);

			vertices.resize(maxVertices);

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * maxVertices, vertices.data(), GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		Batch::~Batch()
		{
			glDeleteBuffers(1, &VBO);

			vertices.clear();
		}

		void Batch::draw(const std::vector<Vertex>& vertices)
		{
			// TODO (eeneku): Check if vertices is full
			for (auto vertex : vertices)
					this->vertices[currentVertex++] = vertex;
		}

		void Batch::flush(const Camera& camera)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, position)));
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, color)));

			shader->bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * currentVertex, (GLvoid*)(vertices.data()));

			GLuint uniformMVP = glGetUniformLocation(shader->getProgram(), "MVP");

			glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(camera.getVP()));

			glDrawArrays(GL_TRIANGLES, 0, currentVertex);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			shader->unbind();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void Batch::drawTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, const Color& color, GLfloat depth)
		{
			std::vector<Vertex> vertices = {
				Vertex(glm::vec3(x1, y1, depth), glm::vec4(color.r, color.g, color.b, color.a)),
				Vertex(glm::vec3(x2, y2, depth), glm::vec4(color.r, color.g, color.b, color.a)),
				Vertex(glm::vec3(x3, y3, depth), glm::vec4(color.r, color.g, color.b, color.a)) };

			draw(vertices);
		}

		void Batch::drawQuad(GLfloat x, GLfloat y, GLfloat width, GLfloat height, const Color& color, GLfloat depth)
		{
			std::vector<Vertex> vertices = {
				Vertex(glm::vec3(x, y + height, depth), glm::vec4(color.r, color.g, color.b, color.a)), 
				Vertex(glm::vec3(x + width, y + height, depth), glm::vec4(color.r, color.g, color.b, color.a)), 
				Vertex(glm::vec3(x + width, y, depth), glm::vec4(color.r, color.g, color.b, color.a)), 
				Vertex(glm::vec3(x, y + height, depth), glm::vec4(color.r, color.g, color.b, color.a)), 
				Vertex(glm::vec3(x, y, depth), glm::vec4(color.r, color.g, color.b, color.a)), 
				Vertex(glm::vec3(x + width, y, depth), glm::vec4(color.r, color.g, color.b, color.a)) };

			draw(vertices);
		}
	}
}