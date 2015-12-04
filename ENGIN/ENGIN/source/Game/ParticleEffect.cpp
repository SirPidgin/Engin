#include "Engin\Game\ParticleEffect.h"

//void Sprite::init(const std::string &texturepath, float textureResolution, float x, float y)
//{
//	this->texturepath = texturepath;
//	this->textureResolution = textureResolution;
//	this->x = x;
//	this->y = y;
//
//	M = glm::translate(glm::vec3(x, y, 0.0f));
//
//	glGenVertexArrays(1, &VertexArrayID);
//	glBindVertexArray(VertexArrayID);
//
//	programID = LoadShaders("spriteVertexShader.vertexshader", "spriteFragmentShader.fragmentshader");
//	MVP_MatrixID = glGetUniformLocation(programID, "MVP");
//
//	TextureID = glGetUniformLocation(programID, "myTextureSampler");
//	Texture = loadBMP_custom(texturepath.c_str());
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//}
//
//Sprite::~Sprite()
//{
//}
//
//void Sprite::render(Camera &cam)
//{
//	glm::mat4 MVP = cam.getVP()*M;
//	glUseProgram(programID);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, Texture);
//	// Set our "myTextureSampler" sampler to user Texture Unit 0
//	glUniform1i(TextureID, 0);
//
//	glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);
//	glVertexAttrib3f(VERTEX_POSITION, 0.0f, 0.0f, 0.0f);
//
//	glPointSize(textureResolution);
//	glDrawArrays(GL_POINTS, 0, 1);
//}

namespace Engin
{
	namespace Renderer
	{
		ParticleEffect::ParticleEffect() : distribution(1.0, 3.0)
		{
		}


		ParticleEffect::~ParticleEffect()
		{
		}

		void ParticleEffect::init(GLuint textureID)
		{
			this->textureID = textureID;
			timeUntilNextStep = 0;
			angle = 0;
			particleVertices.reserve(NUM_PARTICLES);

			for (int i = 0; i < NUM_PARTICLES; i++)
			{
				createParticle(particles + i);
				Particle* p = particles + i;
				p->position = glm::vec3(0.0f, 0.0f, 0.0001f * i);
			}

			for (int i = 0; i < 5 / TIMESTEP; i++)
			{
				step();
			}

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * particleVertices.size(), particleVertices.data(), GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void ParticleEffect::createParticle(Particle* p)
		{
			float randNum = distribution(generator);

			p->position = glm::vec3(0.0f);
			p->velocity = glm::vec2(1.0f * randNum * cos(angle) + 1.0f);
			p->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
			p->age = 0.0f;
			p->lifetime = randNum + 1;
		}

		bool ParticleEffect::compareParticles(Particle* p1, Particle* p2)
		{
			return p1->position[2] < p2->position[2];
		}


		void ParticleEffect::step()
		{
			angle += 0.5f * TIMESTEP;

			while (angle > 2 * PI)
			{
				angle -= 2 * PI;
			}

			for (int i = 0; i < NUM_PARTICLES; i++)
			{
				Particle* p = particles + i;
				p->position += glm::vec3(p->velocity * TIMESTEP, 0.0f);
				p->velocity += glm::vec2(0.0f, -GRAVITY * TIMESTEP);
				p->age += TIMESTEP * 3;

				if (p->age > p->lifetime) { createParticle(p); }
			}
		}

		void ParticleEffect::advance(float dt)
		{
			while (dt > 0)
			{
				if (timeUntilNextStep < dt)
				{
					dt -= timeUntilNextStep;
					step();
					timeUntilNextStep = TIMESTEP;
				}
				else
				{
					timeUntilNextStep -= dt;
					dt = 0;
				}
			}
		}

		void ParticleEffect::draw()
		{
			std::vector<Particle*> ps;
			for (int i = 0; i < NUM_PARTICLES; i++)
			{
				ps.push_back(particles + i);
			}
			std::sort(ps.begin(), ps.end());

			step();

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBegin(GL_QUADS);

			for (unsigned int i = 0; i < ps.size(); i++)
			{
				Particle* p = ps[i];
				glColor4f(p->color[0], p->color[1], p->color[2], 1 - p->age / p->lifetime);
				float size = SIZE / 2;

				glTexCoord2f(0, 1);
				glVertex2f(p->position[0] - size, p->position[1] - size);
				glTexCoord2f(0, 0);
				glVertex2f(p->position[0] - size, p->position[1] + size);
				glTexCoord2f(1, 0);
				glVertex2f(p->position[0] + size, p->position[1] + size);
				glTexCoord2f(1, 1);
				glVertex2f(p->position[0] + size, p->position[1] - size);
			}
			glEnd();
		}
	}
}