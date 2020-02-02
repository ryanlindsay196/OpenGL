#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
class Particle
{
public:
	Particle();
	~Particle();

private:
	//CPU representation of a particle
	struct ParticleInstance {
		glm::vec3 position, speed;
		unsigned char r, g, b, a;//Color
		float size, angle, weight;
		float life; //Remaining life of the particle. if < 0 : dead and unused
	};

	int LastUsedParticle = 0;

	const int MaxParticles = 100000;
	ParticleInstance particleInstances;
	void Update(float gameTime);
	void Draw();

	int FindUnusedParticle();

	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
};

