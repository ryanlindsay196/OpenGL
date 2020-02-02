#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2.h>

enum TextureTypes {albedo, normal};

class Texture
{
private:
	GLuint* m_id;
public:
	GLuint LoadTexture(char* imagepath);
	void InitID(GLuint* in_id);
	GLuint* GetID();

	//enum TextureTypes {none, albedo, normal};
};
//image loading function (string imagePath)
//texture variable