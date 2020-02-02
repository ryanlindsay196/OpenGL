#include "Texture.h"
#include <iostream>

void Texture::InitID(GLuint* in_id)
{
	m_id = in_id;
}

GLuint* Texture::GetID()
{
	return m_id;
}

GLuint Texture::LoadTexture(char* imagepath) {

	int width = 0;
	int height = 0;

	//std::cout << sizeof(GLuint*) << "\n" << sizeof(unsigned char*);

	GLuint* image = (GLuint*)SOIL_load_image(imagepath, &width, &height, NULL, SOIL_LOAD_RGBA);

	glGenTextures(1, image);
	glBindTexture(GL_TEXTURE_2D, *image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		//std::cout << "Texture Loading " << imagepath << " Succeeded\n";
	}
	else
	{
		//std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << imagepath << "\n";
	}

	//glActiveTexture(*image);
	//glBindTexture(GL_TEXTURE_2D, *image);
	//SOIL_free_image_data(image);

	//m_Texture = (GLuint*)image;
	m_id = image;
	return (GLuint)image;
}