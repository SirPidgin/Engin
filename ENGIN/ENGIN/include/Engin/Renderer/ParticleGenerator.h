#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

#include <Engin\Resources\ShaderProgram.h>
#include <Engin\Resources\Texture.h>

struct Particle
{
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec4 color;
	GLfloat life;

	Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
};

// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.

namespace Engin
{
	namespace Renderer
	{
		class ParticleGenerator // TODO <Pidgin> - IMPLEMENTATION (Game Objects?)
		{
		public:
			ParticleGenerator(GLuint amount);
			~ParticleGenerator();

		private:
			std::vector<Particle> particles;
			GLuint amount;
		};
	}
}
