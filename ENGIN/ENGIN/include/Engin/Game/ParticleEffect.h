#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include "Engin\Resources\ShaderProgram.h"
#include "Engin\Resources\Texture.h"

struct Particle
{
	glm::vec2 position;		// Center point of particle.
	glm::vec2 velocity;		// Velocity of particle in 2D space.
	glm::vec4 color;		// 4-component color (RGBA).
	GLfloat rotation;		// Rotation to be applied on z-axis.
	GLfloat age;			// How long the particle has lived (seconds).
	GLfloat lifetime;		// How long the particle will live. (seconds)

	Particle() :
		position(0.0f), velocity(0.0f), color(0.0f),
		rotation(0.0f), age(0.0f), lifetime(0.0f)
	{}
};

const float GRAVITY = 3.0f;
const int NUM_PARTICLES = 1000;
const float TIMESTEP = 0.01f;
const float SIZE = 0.05f;
const float PI = 3.1316;

// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.

namespace Engin
{
	namespace Renderer
	{
		class ParticleEffect
		{
		public:
			ParticleEffect();
			~ParticleEffect();

			void init(GLuint textureID);
			glm::vec2 curVelocity();
			void createParticle(Particle* p);
			bool compareParticles(Particle* p1, Particle* p2);

			void step();
			void advance(float dt);
			void draw();

		private:
			GLuint textureID;
			Particle particles[NUM_PARTICLES];
			float timeUntilNextStep;
			float angle;

			std::default_random_engine generator;
			std::uniform_real_distribution<float> distribution;
		};
	}
}
