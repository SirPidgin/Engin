#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include "Engin\Resources\ShaderProgram.h"
#include "Engin\Resources\Texture.h"
#include "Engin\Renderer\TextureBatch.h"
#include "Engin\Game\GameObject.h"
#include "Engin\Game\Component.h"

struct Particle
{
	glm::vec3 position;		// Center point of particle.
	glm::vec4 color;		// 4-component color (RGBA).
	glm::vec2 velocity;		// Velocity of particle in 2D space.
	GLfloat rotation;		// Rotation to be applied on z-axis.
	GLfloat age;			// How long the particle has lived (seconds).
	GLfloat lifetime;		// How long the particle will live. (seconds)

	Particle() :
		position(0.0f), velocity(0.0f), color(0.0f),
		rotation(0.0f), age(0.0f), lifetime(0.0f) {}
};

struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
	
	Vertex() :
		position(0.0f), color(0.0f) {}
	Vertex(float x, float y, float z, float r, float g, float b, float a) :
		position(x, y, z), color(r, g, b, a) {}
};


const float GRAVITY = 2.0f;
const int NUM_PARTICLES = 1000;
const float TIMESTEP = 0.01f;
const float SIZE = 0.007f;
const float PI = 3.1316f;

// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.

namespace Engin
{
	namespace Game
	{
		class ParticleEffect : public Component
		{
		public:
			ParticleEffect(GameObject* ownerObject);
			ParticleEffect();
			~ParticleEffect();

			void init(Resources::Texture* texture);
			void createParticle(Particle* p);
			bool compareParticles(Particle* p1, Particle* p2);

			void step();
			void advance(float dt);
			void draw();

		private:
			Resources::Texture* currentTexture;
			Particle particles[NUM_PARTICLES];
			float timeUntilNextStep;
			float angle;

			std::default_random_engine generator;
			std::uniform_real_distribution<float> distribution;
			std::vector<Vertex> particleVertices;
			Renderer::TextureBatch* textureBatch;
		};
	}
}