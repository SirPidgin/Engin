#include "Engin\Renderer\TextureBatch.h"

namespace Engin
{
	namespace Renderer
	{

		TextureBatch::TextureBatch() : shader(nullptr), IBO(0), VBO(0), textureQueueCount(0), textureQueueArraySize(0)
		{
			createBuffers();
		}

		TextureBatch::~TextureBatch()
		{
			glDeleteBuffers(1, &IBO);
			glDeleteBuffers(1, &VBO);
		}

		void TextureBatch::createBuffers()
		{
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * maxBatchSize * vertiecsPerTexture, nullptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			vertices.reserve(initialQueueSize * vertiecsPerTexture);

			createIndexValues();

			glGenBuffers(1, &IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void TextureBatch::createIndexValues()
		{
			size_t size = maxBatchSize * indicesPerTexture;
			indices.reserve(size);

			for (unsigned short i = 0; i < size; i += vertiecsPerTexture)
			{
				indices.push_back(i);
				indices.push_back(i + 1);
				indices.push_back(i + 2);

				indices.push_back(i + 1);
				indices.push_back(i + 3);
				indices.push_back(i + 2);
			}
		}

		void TextureBatch::prepareForRendering()
		{
		}

		void TextureBatch::growTextureQueue()
		{
		}

		void TextureBatch::growSortedTextures()
		{
		}

		void TextureBatch::sortTextures()
		{
		}

		void TextureBatch::renderBatch()
		{
		}

		void TextureBatch::renderSprite()
		{
		}

		void TextureBatch::draw(Resources::Texture* texture, glm::vec4* textureRegion, float x, float y, float width, float height, float rotation, float scale, const Color& color, float opacity, float depth)
		{
			if (textureQueueCount >= textureQueueArraySize)
			{
				growTextureQueue();
			}

			TextureInfo* textureInfo = &textureQueue[textureQueueCount];

			if (textureRegion)
			{
				textureInfo->texCoords = *textureRegion;
			}
			else
			{
				//textureInfo->texCoords = glm::vec4(0.0f, 0.0f, (GLfloat)texture->getWidth(), (GLfloat)texture->getHeight());
			}

			textureInfo->color = glm::vec4(color.r, color.b, color.g, opacity);
			textureInfo->depth = depth;

			textureInfo->topLeft = glm::vec2(x, y + height);
			textureInfo->topRight = glm::vec2(x + width, y + height);
			textureInfo->bottomLeft = glm::vec2(x, y);
			textureInfo->bottomLeft = glm::vec2(x + width, y);

			textureQueueCount++;
		}

		void TextureBatch::flush(const Camera& camera)
		{
		}
	}
}