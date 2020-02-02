#pragma once
#include <vector>
#include "glm.hpp"


class Text2D
{
private:
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
public:
	void initText2D(const char* texturePath);
	void printText2D(const char* text, int x, int y, int size);
	void cleanupText2D();
};