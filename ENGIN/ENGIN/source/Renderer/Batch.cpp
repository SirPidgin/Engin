#include "Engin\Renderer\Batch.h"

#include <cassert>
#include <glm\gtc\type_ptr.hpp>

namespace Engin
{
	namespace Renderer
	{
		Batch::Batch() : 
			VBO(0), 
			currentVertex(0), 
			shader(nullptr)
		{

		}

		Batch::~Batch()
		{
			glDeleteBuffers(1, &VBO);

			vertices.clear();
		}

		// Initialises the batch.
		void Batch::init(Resources::ShaderProgram* shader, size_t maxVertices)
		{
			this->shader = shader;

			assert(maxVertices > 0); // Maximum number of vertices has to be larger than 0.

			vertices.resize(maxVertices);

			// Generates the vertex buffer.
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * maxVertices, vertices.data(), GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Adds given vertices to the batch.
		void Batch::draw(const std::vector<Vertex>& vertices)
		{
			// We need to check if there is room for the vertices.
			if (currentVertex + vertices.size() < this->vertices.size())
			{
				for (auto vertex : vertices)
					this->vertices[currentVertex++] = vertex;
			}
		}

		// Flushes (renders) the batch.
		void Batch::flush(const Camera& camera)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds the vertex buffer.

			// Enables vertex attribute arrays used in the rendering.
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			// Sets the pointers to vertex attributes.
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, position)));
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, color)));

			shader->bind(); // Binds shader.

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * currentVertex, (GLvoid*)(vertices.data())); // Updates the vertex data in GPU memory.

			glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(camera.getVP())); // Sends the camera's view and projection matrix the shaders.

			glDrawArrays(GL_TRIANGLES, 0, currentVertex); // Draws the vertices.

			// Disables vertex attribute arrays and unbinds shader and vertex buffer.
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			shader->unbind();
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Draws a triangle.
		void Batch::drawTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, const Color& color, GLfloat opacity, GLfloat depth)
		{
			// Creates vertices from given parameters.
			std::vector<Vertex> vertices = {
				Vertex(glm::vec3(x1, y1, depth), glm::vec4(color.r, color.g, color.b, opacity)),
				Vertex(glm::vec3(x2, y2, depth), glm::vec4(color.r, color.g, color.b, opacity)),
				Vertex(glm::vec3(x3, y3, depth), glm::vec4(color.r, color.g, color.b, opacity)) };

			draw(vertices); // Adds vertices to the batch.
		}

		//Draws a quad.
		void Batch::drawQuad(GLfloat x, GLfloat y, GLfloat width, GLfloat height, const Color& color, GLfloat opacity, GLfloat depth)
		{
			// Creates vertices from given parameters.
			std::vector<Vertex> vertices = {
				Vertex(glm::vec3(x, y + height, depth), glm::vec4(color.r, color.g, color.b, opacity)),
				Vertex(glm::vec3(x + width, y + height, depth), glm::vec4(color.r, color.g, color.b, opacity)),
				Vertex(glm::vec3(x + width, y, depth), glm::vec4(color.r, color.g, color.b, opacity)),
				Vertex(glm::vec3(x, y + height, depth), glm::vec4(color.r, color.g, color.b, opacity)),
				Vertex(glm::vec3(x, y, depth), glm::vec4(color.r, color.g, color.b, opacity)),
				Vertex(glm::vec3(x + width, y, depth), glm::vec4(color.r, color.g, color.b, opacity)) };

			draw(vertices); // Adds vertices to the batch.
		}
	}
}