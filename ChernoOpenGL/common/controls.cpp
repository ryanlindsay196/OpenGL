#include "controls.hpp"
#include <iostream>
//class Controls
//{

	//glm::vec3 position;
	//float horizontalAngle;
	//float verticalAngle;
	//float initialFoV;
	//float speed;
	//float mouseSpeed;
	//glm::vec3 direction;
	//glm::vec3 right;
	//glm::vec3 up;

	void Controls::InitializeControls(bool lockX, bool lockY, bool lockZ)
	{
		//position
		position = glm::vec3(0, 0, 5);

		//horizontal angle : toward -Z
		horizontalAngle = 3.14f;

		//vertical angle : 0, look at the horizon
		verticalAngle = 0.0f;

		//Initial Field of View
		initialFoV = 45.0f;

		speed = 40.0f;//3 units / second
		mouseSpeed = 0.55f;

		direction = glm::vec3(0, 0, 0);

		sceneData.cameraLockAxes.lockX = lockX;
		sceneData.cameraLockAxes.lockY = lockY;
		sceneData.cameraLockAxes.lockZ = lockZ;
	}


	void Controls::ComputeMatricesFromInputs(GLFWwindow* window, float deltaTime)
	{


		//Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		//Reset mouse position for next frame
		glfwSetCursorPos(window, 1024 / 2, 768 / 2);

		//figure this out
		//float FoV = initialFoV - 5 * glfwSetScrollCallback(window, scroll_callback);

		//Compute new orientation
		if(!sceneData.cameraLockAxes.lockX)
			horizontalAngle += mouseSpeed * deltaTime * float(1024 / 2 - xpos);
		if(!sceneData.cameraLockAxes.lockY)
			verticalAngle += mouseSpeed * deltaTime * float(768 / 2 - ypos);

		//Clamp vertical angle between two values
		verticalAngle = (float)std::fmin(verticalAngle, 0.64f);
		verticalAngle = (float)std::fmax(-0.64, verticalAngle);
		
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		//Right vector
		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		//Up vector : perpendicular to both direction and right
		up = glm::cross(right, direction);

		//Move forward
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			position += direction * deltaTime * speed;
		}
		//Move badkward
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			position -= direction * deltaTime * speed;
		}
		//Strafe right
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			position += right * deltaTime * speed;
		}
		//Strafe left
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}

		//std::cout << position << "\n";
		//fprintf(stderr, "Failed to open GLFW window. If you have an Inten GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");

		//return position;
		//return glm::vec3();
	}
//}