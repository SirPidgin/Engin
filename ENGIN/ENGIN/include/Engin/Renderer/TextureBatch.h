#pragma once

#include <memory>
#include <vector>
#include "GL\glew.h"
#include "glm\glm.hpp"
#include "Engin\Resources\Shader.h"
#include "Engin\Resources\Texture.h"
#include "Engin\Renderer\Color.h"
#include "Engin\Renderer\Camera.h"

namespace Engin
{
	namespace Renderer
	{
		class TextureBatch
		{
		public:
			TextureBatch();
			~TextureBatch();

			void setShader(Resources::Shader* shader) { this->shader = shader; }
			void draw(Resources::Texture* texture, glm::vec4* textureRegion, float x, float y, float width, float height, float rotation, float scale, const Color& color, float opacity = 1.0f, float depth = 0.0f);
			void flush(const Camera& camera);

			void clear() { textureQueueCount = 0; }
		private:
			void createBuffers();
			void createIndexValues();
			void prepareForRendering();
			void growTextureQueue();
			void growSortedTextures();
			void sortTextures();
			void renderBatch();
			void renderSprite();

			struct TextureInfo
			{
				glm::vec4 texCoords;
				glm::vec4 color;
				glm::vec2 topLeft;
				glm::vec2 topRight;
				glm::vec2 bottomLeft;
				glm::vec2 bottomRight;
				const Resources::Texture* texture;
				float depth;
			};

			struct Vertex
			{
				glm::vec4 color;
				glm::vec3 position;
				glm::vec2 uv;

				Vertex() : position(0.0f), uv(0.0f), color(0.0f) {}
				Vertex(float x, float y, float z, float r, float g, float b, float a, float u, float v)
					: position(x, y, z), color(r, g, b, a), uv(u, v) {}
			};

			std::unique_ptr<TextureInfo[]> textureQueue;

			static const size_t maxBatchSize = 2048;
			static const size_t minBatchSize = 128;
			static const size_t initialQueueSize = 64;
			static const size_t vertiecsPerTexture = 4;
			static const size_t indicesPerTexture = 6;

			size_t textureQueueCount;
			size_t textureQueueArraySize;

			GLuint VBO;
			GLuint IBO;

			std::vector<TextureInfo const*> sortedTextures;

			std::vector<Vertex> vertices;
			std::vector<GLushort> indices;

			Resources::Shader* shader;
		};
	}
}
