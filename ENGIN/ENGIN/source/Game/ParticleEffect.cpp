#include "Engin\Game\ParticleEffect.h"


namespace Engin
{
	namespace Game
	{
		ParticleEffect::ParticleEffect(GameObject* ownerObject) :
			distribution(1.0, 3.0),
			Component(ownerObject),
			currentTexture(nullptr)
		{
			this->textureBatch = ownerObject->getTextureBatch();
		}

		ParticleEffect::ParticleEffect() :
			distribution(1.0, 3.0)
		{
		}


		ParticleEffect::~ParticleEffect()
		{
		}

		void ParticleEffect::init(Resources::Texture* texture)
		{
			currentTexture = texture;
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
		}

		void ParticleEffect::createParticle(Particle* p)
		{
			float randNum = distribution(generator);

			p->position = 
				glm::vec3(ownerObject->accessComponent<Transform>()->getPosition(),
				ownerObject->accessComponent<Transform>()->getDepth());
			p->velocity = (glm::vec2(glm::cos(ownerObject->accessComponent<Transform>()->getRotation()), glm::sin(ownerObject->accessComponent<Transform>()->getRotation()))  * 500.0f);
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
				p->position += glm::vec3(ownerObject->accessComponent<Transform>()->getPosition(), 0.0f);
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

			for (unsigned int i = 0; i < ps.size(); i++)
			{
				Particle* p = ps[i];
				glColor4f(p->color[0], p->color[1], p->color[2], 1 - p->age / p->lifetime);
				float size = SIZE / 2;

				textureBatch->draw(currentTexture, nullptr,
					p->position.x, p->position.y,
					currentTexture->getWidth(), currentTexture->getHeight(),
					currentTexture->getWidth() / 2.0f, currentTexture->getHeight() / 2.0f,
					p->rotation, ownerObject->accessComponent<Transform>()->getScale() ,
					Renderer::clrWhite, 1.0f, p->position.z);
			}
		}
	}
}