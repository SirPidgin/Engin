#include "Engin\Renderer\TextureBatch.h"

#include <algorithm>
#include <cassert>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\rotate_vector.hpp>

namespace Engin
{
	namespace Renderer
	{

		TextureBatch::TextureBatch() : 
			shader(nullptr), 
			IBO(0), 
			VBO(0), 
			textureQueueCount(0), 
			textureQueueArraySize(0), 
			vertexBufferPos(0), 
			inBeginEndPair(false), 
			sortMode(TextureSortMode::Texture)
		{
			createBuffers();
		}

		TextureBatch::~TextureBatch()
		{
			glDeleteBuffers(1, &IBO);
			glDeleteBuffers(1, &VBO);

			vertices.clear();
			indices.clear();
			sortedTextures.clear();
		}

		// Begins the rendering block.
		void TextureBatch::begin()
		{
			inBeginEndPair = true;
		}

		// Override for the default draw method for simpler usage.
		void TextureBatch::draw(Resources::Texture* texture, float x, float y, float opacity, float depth)
		{
			draw(texture, nullptr, x, y, texture->getWidth(), texture->getHeight(), texture->getWidth() / 2.0f, texture->getHeight() / 2.0f, 0.0f, 1.0f, Renderer::clrWhite, opacity, depth);
		}

		// Override for the default draw method for simpler usage.
		void TextureBatch::draw(Resources::Texture* texture, float x, float y, float width, float height, const Color& color, float opacity, float depth)
		{
			draw(texture, nullptr, x, y, width, height, width / 2.0f, height / 2.0f, 0.0f, 1.0f, color, opacity, depth);
		}

		// Override for the default draw method for simpler usage.
		void TextureBatch::draw(Resources::Texture* texture, glm::vec4* textureRegion, float x, float y, float width, float height, float rotation, float scale, const Color& color, float opacity, float depth)
		{
			draw(texture, nullptr, x, y, width, height, width / 2.0f, height / 2.0f, 0.0f, 1.0f, color, opacity, depth);
		}

		// The default draw method. Adds the given texture with the given parameters to the batch.
		void TextureBatch::draw(Resources::Texture* texture, glm::vec4* textureRegion, float x, float y, float width, float height, float rotateOriginX, float rotateOriginY, float rotation, float scale, const Color& color, float opacity, float depth)
		{
			// Grow the texture queue if needed.
			if (textureQueueCount >= textureQueueArraySize)
			{
				growTextureQueue();
			}

			TextureInfo* textureInfo = &textureQueue[textureQueueCount]; // Get pointer to the texture info we will be using.

			// Get the texture region if given.
			if (textureRegion)
			{
				textureInfo->texCoords.x = textureRegion->x / texture->getWidth();
				textureInfo->texCoords.y = textureRegion->y / texture->getHeight();
				textureInfo->texCoords.z = textureInfo->texCoords.x + textureRegion->z / texture->getWidth();
				textureInfo->texCoords.w = textureInfo->texCoords.y + textureRegion->w / texture->getHeight();
			}
			else
			{
				textureInfo->texCoords = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Default region is the whole texture.
			}

			// Scale the position of the texture by the scale parameter.
			rotateOriginX *= scale;
			rotateOriginY *= scale;

			// Scale the size of the texture by the scale parameter.
			width = width * scale;
			height = height * scale;

			// Add the details to the texture info.
			textureInfo->texture = texture;
			textureInfo->scale = scale;
			textureInfo->depth = depth;
			textureInfo->color = glm::vec4(color.r, color.g, color.b, opacity);
			textureInfo->topLeft = glm::vec2(x, y) + glm::rotate(glm::vec2(-rotateOriginX, height - rotateOriginY), rotation);
			textureInfo->topRight = glm::vec2(x, y) + glm::rotate(glm::vec2(width - rotateOriginX, height - rotateOriginY), rotation);
			textureInfo->bottomLeft = glm::vec2(x, y) + glm::rotate(glm::vec2(-rotateOriginX, -rotateOriginY), rotation);
			textureInfo->bottomRight = glm::vec2(x, y) + glm::rotate(glm::vec2(width - rotateOriginX, -rotateOriginY), rotation);

			textureQueueCount++; // Grow the texture queue count.
		}

		// Ends the rendering block.
		void TextureBatch::end()
		{
			assert(inBeginEndPair);
			
			// Sort the textures and prepare for rendering.
			sortTextures();
			prepareForRendering();

			inBeginEndPair = false;
		}

		// Flushes (renders) the batch.
		void TextureBatch::flush(const Camera& camera)
		{
			// Return if the queue is empty or if called during rendering block.
			if (!textureQueueCount && !inBeginEndPair)
			{
				return;
			}

			// Bind buffers and shader.
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

			shader->bind();

			/// Enable vertex attribute arrays and set attribute pointers.
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, color)));
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, position)));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, uv)));

			// Set uniforms.
			glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(camera.getVP()));
			glUniform1i(glGetUniformLocation(shader->getProgram(), "ourTexture"), 0);

			Resources::Texture* batchTexture = nullptr;
			size_t batchStart = 0;

			// Loop the textures.
			for (size_t i = 0; i < textureQueueCount; i++)
			{
				Resources::Texture* texture = sortedTextures[i]->texture; // Get pointer to the current texture.

				// Render batch if texture changes.
				if (texture != batchTexture)
				{
					if (i > batchStart)
					{
						renderBatch(batchTexture, batchStart, i - batchStart, camera);
					}

					batchTexture = texture;
					batchStart = i;
				}
			}

			renderBatch(batchTexture, batchStart, textureQueueCount - batchStart, camera); // Render last elements.

			// Disable vertex attribute arrays.
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

			// Unbind buffers and shader.
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			shader->unbind();

			vertexBufferPos = 0; // Reset vertex buffer position.
		}

		// Clears the batch.
		void TextureBatch::clear()
		{
			textureQueueCount = 0;
			vertexBufferPos = 0;
			sortedTextures.clear();
		}

		// Creates buffers used in rendering.
		void TextureBatch::createBuffers()
		{
			// Creates the vertex buffer.
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * maxBatchSize * verticesPerTexture, nullptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			createVertices(); // Fills the vertex array with empty data.
			createIndexValues(); // Fills the index array.

			// Creates the index buffer.
			glGenBuffers(1, &IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		// Creates empty vertices.
		void TextureBatch::createVertices()
		{
			size_t size = maxBatchSize * verticesPerTexture;
			vertices.reserve(size);

			for (size_t i = 0; i < size; i++)
			{
				vertices.push_back(Vertex());
			}
		}

		// Creates indices.
		void TextureBatch::createIndexValues()
		{
			// Indices are the same on all textures.
			// We can calculate them in the beforehand.
			size_t size = maxBatchSize * indicesPerTexture;
			indices.reserve(size);

			for (unsigned short i = 0; i < size; i += verticesPerTexture)
			{
				indices.push_back(i);
				indices.push_back(i + 1);
				indices.push_back(i + 2);

				indices.push_back(i + 1);
				indices.push_back(i + 3);
				indices.push_back(i + 2);
			}
		}

		// Prepares the batch for rendering.
		void TextureBatch::prepareForRendering()
		{
			const TextureInfo* t = nullptr;

			// Creates vertices of all textures in the queue.
			for (size_t i = 0, j = 0; i < textureQueueCount; i++)
			{
				t = sortedTextures[i];
				vertices[j++].set(
					t->topLeft.x, t->topLeft.y, t->depth, 
					t->color.r, t->color.g, t->color.b, t->color.a, 
					t->texCoords.x, t->texCoords.y);

				vertices[j++].set(
					t->topRight.x, t->topRight.y, t->depth,
					t->color.r, t->color.g, t->color.b, t->color.a,
					t->texCoords.z, t->texCoords.y);
	
				vertices[j++].set(
					t->bottomLeft.x, t->bottomLeft.y, t->depth,
					t->color.r, t->color.g, t->color.b, t->color.a,
					t->texCoords.x, t->texCoords.w);

				vertices[j++].set(
					t->bottomRight.x, t->bottomRight.y, t->depth,
					t->color.r, t->color.g, t->color.b, t->color.a,
					t->texCoords.z, t->texCoords.w);
			}

			// Updates the vertex data in GPU memory.
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * textureQueueCount * verticesPerTexture, (void*)(vertices.data()));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Grows the texture queue.
		void TextureBatch::growTextureQueue()
		{
			size_t newSize = std::max(initialQueueSize, textureQueueArraySize * 2); // Grow by power of two.

			// Create a new array and copy the old data.
			std::unique_ptr<TextureInfo[]> newArray(new TextureInfo[newSize]);

			for (size_t i = 0; i < textureQueueCount; i++)
			{
				newArray[i] = textureQueue[i];
			}

			textureQueue = std::move(newArray);
			textureQueueArraySize = newSize;

			sortedTextures.clear();
		}

		// Grow the size of sorted textures.
		void TextureBatch::growSortedTextures()
		{
			size_t previousSize = sortedTextures.size();

			sortedTextures.resize(textureQueueCount);

			for (size_t i = previousSize; i < textureQueueCount; i++)
			{
				sortedTextures[i] = &textureQueue[i];
			}
		}

		// Sorts the textures.
		void TextureBatch::sortTextures()
		{
			// Check if we need to grow the sorted textures.
			if (sortedTextures.size() < textureQueueCount)
			{
				growSortedTextures();
			}

			// Sort textures by the sort mode.
			switch (sortMode)
			{
				// Sort by texture. 
				case TextureSortMode::Texture:
				{
					std::sort(sortedTextures.begin(), sortedTextures.begin() + textureQueueCount, [](TextureInfo const* x, TextureInfo const* y) -> bool
					{
						return x->texture < y->texture;
					});
				} break;

				// Sort from back to front.
				case TextureSortMode::BackToFront:
				{
					std::sort(sortedTextures.begin(), sortedTextures.begin() + textureQueueCount, [](TextureInfo const* x, TextureInfo const* y) -> bool
					{
						return x->depth > y->depth;
					});
				} break;

				// Sort from front to back.
				case TextureSortMode::FrontToBack:
				{
					std::sort(sortedTextures.begin(), sortedTextures.begin() + textureQueueCount, [](TextureInfo const* x, TextureInfo const* y) -> bool
					{
						return x->depth < y->depth;
					});
				} break;

				default: break;
			}
		}

		// Renders the batch.
		void TextureBatch::renderBatch(Resources::Texture* texture, size_t start, size_t count, const Camera& camera)
		{
			texture->bind(GL_TEXTURE0 + 0); // Binds texture.

			while (count > 0)
			{
				size_t batchSize = count;
				size_t remainingSpace = maxBatchSize - vertexBufferPos;

				if (batchSize > remainingSpace)
				{
					if (remainingSpace < minBatchSize)
					{
						vertexBufferPos = 0;
						batchSize = std::min(count, maxBatchSize);
					}
					else
					{
						batchSize = remainingSpace;
					}
				}

				glDrawElements(GL_TRIANGLES, indicesPerTexture * batchSize, GL_UNSIGNED_SHORT, (void*)((indicesPerTexture * vertexBufferPos * sizeof(unsigned short))));
			
				count -= batchSize;

				vertexBufferPos += batchSize;
			}

			texture->unbind();
		}
	}
}