#include "Particle.h"

//The VBO containing the 4 vertices of the particles.
//Thanks to instancing, they will be shared by all particles.
static const GLfloat g_vertex_buffer_data[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f
};

Particle::Particle()
{
	//MaxParticles = 512;
	//billboard vertex buffer
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	//Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	//The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	//Initialize with empty (NULL) buffer: it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}


Particle::~Particle()
{
}

void Particle::Update(float gameTime)
{
	int ParticlesCount = 0;
	//for (int i = 0; i < MaxParticles; i++)
	//{
	//	ParticleInstance& p = particleInstances[i];
	//
	//	if (p.life > 0.0f)
	//	{
	//		p.life -= gameTime;
	//		if (p.life > 0.0f) {
	//			//Simulate simple physics : gravity only, no collisions
	//			p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)gameTime * 0.5f;
	//			p.position += p.speed * (float)gameTime;
	//			p.cameradistance = glm::length(p.position - cameraPosition);
	//
	//			//Fill the GPU buffer
	//			g_particle_position_size_data[4 * ParticlesCount + 0] = p.position.x;
	//			g_particle_position_size_data[4 * ParticlesCount + 1] = p.position.y;
	//			g_particle_position_size_data[4 * ParticlesCount + 2] = p.position.z;
	//
	//			g_particle_position_size_data[4 * ParticlesCount + 3] = p.size;
	//
	//			g_particle_color_data[4 * ParticlesCount + 0] = p.r;
	//			g_particle_color_data[4 * ParticlesCount + 1] = p.g;
	//			g_particle_color_data[4 * ParticlesCount + 2] = p.b;
	//			g_particle_color_data[4 * ParticlesCount + 3] = p.a;
	//		}
	//		else
	//		{
	//			//Paricles that just died will be put at the end of the buffer in SortParticles();
	//		}
	//
	//		ParticlesCount++;
	//	}
	//}
	//
	//SortParticles();
	//
	////Update the buffers that OpenGL uses for rendering.
	////There are much more sophisticated means to stream data from the CPU to the GPU,
	////but this is outside the scope of this tutorial.
	////http://www.opengl.org/wiki/Buffer_Object_Streaming
	//
	//glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	//glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);//Buffer orphaning, a common way to improve streaming perf. See above link for details.
	//glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particle_position_size_data);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	//glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);//Buffer orphaning, a common way to improve streaming perf. See above link for details.
	//glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particle_color_data);
}

void Particle::Draw()
{
	//1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0, //attribute. No particular reason for 0, but must match the layout in the shader.
		3, //size
		GL_FLOAT, //type
		GL_FALSE, //normalized?
		0, //stride
		(void*)0 //array buffer offset
	);

	//2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1, //attribute. No particular reason for 1, but must match the layout of the shader.
		4, //size : x + y + z + size => 4
		GL_FLOAT, //type
		GL_FALSE, //normalized?
		0, //stride
		(void*)0 //array buffer offset
	);

	//3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		2, //attribute. No particular reason for 2, but must match the layout of the shader.
		4, //size : 4 + g + b + a => 4
		GL_UNSIGNED_BYTE, //type
		GL_TRUE, //normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0, //stride
		(void*)0 //array buffer offset
	);

	//These functions are specific to glDrawArrays *Instanced*.
	//The first parameter is the attribute buffer we're talking about.
	//The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	//http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); //particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); //positions : one per quad (its center) -> 1
	glVertexAttribDivisor(2, 1); //color : one per quad -> 1

	//Draw the particles!
	//This draws many times a small triangle_strip (which looks like a quad).
	//This is equivalent to :
	//for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
	//but faster
	//glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);
}

int Particle::FindUnusedParticle()
{
	//for (int i = LastUsedParticle; i < MaxParticles; i++)
	//{
	//	if (particleInstances[i].life < 0)
	//	{
	//		LastUsedParticle = i;
	//		return i;
	//	}
	//}
	//
	//for (int i = 0; i < LastUsedParticle; i++)
	//{
	//	if (particleInstances[i].life < 0)
	//	{
	//		LastUsedParticle = i;
	//		return i;
	//	}
	//}

	return 0; //ALL particles are taken, override the first one
}