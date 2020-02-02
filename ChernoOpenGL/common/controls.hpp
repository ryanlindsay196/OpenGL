#pragma once

#include <GLFW/glfw3.h>
#include <glm.hpp>
#include "EntityDataStructures.h"

class Controls
{
public:

	//CameraLockAxes cameraLockAxes;
	SceneData sceneData;
	//position
	glm::vec3 position;

	//horizontal angle : toward -Z
	float horizontalAngle;

	//vertical angle : 0, look at the horizon
	float verticalAngle;

	//Initial Field of View
	float initialFoV;

	float speed;//3 units / second
	float mouseSpeed;

	glm::vec3 direction;

	glm::vec3 up;
	glm::vec3 right;

	void InitializeControls(bool lockX, bool lockY, bool lockZ);
	void ComputeMatricesFromInputs(GLFWwindow* window, float deltaTime);
};