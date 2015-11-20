#include "Engin\Game\ParticleEffect.h"

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

			for (int i = 0; i < NUM_PARTICLES; i++)
			{
				createParticle(particles + i);
			}

			for (int i = 0; i < 5 / TIMESTEP; i++)
			{
				step();
			}
		}

		glm::vec2 ParticleEffect::curVelocity()
		{
			return glm::vec2(2 * cos(angle), 2.0f);
		}

		void ParticleEffect::createParticle(Particle* p)
		{
			float randNum = distribution(generator);

			p->position = glm::vec2(0.0f, 0.0f);
			p->velocity = curVelocity() + glm::vec2(0.5f * randNum - 0.25);
			p->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
			p->age = 0.0f;
			p->lifetime = randNum + 1;
		}

		bool ParticleEffect::compareParticles(Particle* p1, Particle* p2)
		{
			return p1->position[1] < p2->position[1];
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
				p->position += p->velocity * TIMESTEP;
				p->velocity += glm::vec2(0.0f, -GRAVITY * TIMESTEP);
				p->age += TIMESTEP;

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