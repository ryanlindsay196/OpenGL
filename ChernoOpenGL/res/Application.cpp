//#include <stdio.h>
//#include <stdlib.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm.hpp>
#include <../common/loadShader.hpp>
//#include <glm/gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include "../common/controls.hpp"
#include <vector>
#include "../common/Model.h"
#include "../common/text2D.h"
#include <gtc/type_ptr.hpp>
#include "../common/ModelManager.h"
#include "../common/EntityManager.h"
#include <Soil2.h>
#include "../common/Entities/Tank.h"
#include "../common/EntityDataStructures.h"

using namespace glm;

int main() {
	glewExperimental = true;//needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);//4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//we want openGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //we don't want the old openGL

	//Open an window and create its OpenGL context
	GLFWwindow* window; //(In the accompanying source code, this variable is global for simplicity)
	window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Inten GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//Initialize GLEW
	glewExperimental = true;//Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);

	//Enable depth test
	glEnable(GL_DEPTH_TEST);
	//Accept the gragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");

	//Projection matrix : 45 degree Field of view, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)1024 / (float)768, 0.1f, 100.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	//Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 0),	//Camera is at (x,y,z), in World Space
		glm::vec3(0, 0, 0),	//and looks at the origin
		glm::vec3(0, 1, 0)	//Head is up (set to 0, -1, 0) to look upside-down)
	);



	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Controls controls;
	//controls.InitializeControls(true, true, false);
	controls.InitializeControls(false, false, false);

	ModelManager* modelManager = new ModelManager();
	EntityManager* entityManager = new EntityManager(programID, modelManager);

#pragma region Load models and textures
	//for (int i = 0; i < 15; i++)
	//{
	//	modelManager->InstantiateModel(programID, (char*)"res/models/Susanne.obj", (char*)"res/textures/SLT_Dif.png", glm::vec3(0, i * 5, 0));
	//	modelManager->InstantiateModel(programID, (char*)"res/models/TutorialModel.obj", (char*)"res/textures/BoxTex.bmp", glm::vec3(5, i * 5, 0));
	//	modelManager->InstantiateModel(programID, (char*)"res/models/TutorialModel.obj", (char*)"res/textures/eggman.bmp", glm::vec3(10, i * 5, 0));
	//	modelManager->InstantiateModel(programID, (char*)"res/models/TutorialModel.obj", (char*)"res/textures/eggman.bmp", glm::vec3(15, i * 5, 0));
	//	modelManager->InstantiateModel(programID, (char*)"res/models/TutorialModel.obj", (char*)"res/textures/eggman.bmp", glm::vec3(20, i * 5, 0));
	//	modelManager->InstantiateModel(programID, (char*)"res/models/TutorialModel.obj", (char*)"res/textures/eggman.bmp", glm::vec3(25, i * 5, 0));
	//}
#pragma endregion

	//Tank tank(modelManager, programID, glm::vec3(0, 4, -3));

	glUseProgram(programID);

#pragma region Initialize Uniforms
	//Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewID = glGetUniformLocation(programID, "V");
	GLuint ProjectionID = glGetUniformLocation(programID, "P");

	GLuint TextureID = glGetUniformLocation(programID, "material.diffuseTex");
	GLuint LightID = glGetUniformLocation(programID, "lightPos");
	GLuint AmbientID = glGetUniformLocation(programID, "material.ambient");
	GLuint CameraID = glGetUniformLocation(programID, "cameraPos");
	//	GLuint DiffuseID = glGetUniformLocation(programID, "material.diffuse");
#pragma endregion
	fvec3 ambientVec = glm::fvec3(0.4, 0.4, 0.4);
	fvec3 diffuseVec = glm::fvec3(1, 1, 1);
	glUniform3fv(glGetUniformLocation(programID, "material.ambient"), 1, glm::value_ptr(ambientVec));
	glUniform3fv(glGetUniformLocation(programID, "material.diffuse"), 1, glm::value_ptr(diffuseVec));
	//glUniform3fv()
	//std::cout << AmbientID << ProjectionID;
	//glUniform3fv(DiffuseID, 1, 1, 1);
	//Text2D uiText;
	//uiText.initText2D("../res/fonts/font.png");

	glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &Projection[0][0]);
	glm::vec2 angleMultiplier = glm::vec2(0, 0);
	float currentTime = (float)glfwGetTime();
	glm::vec3 lightPos = glm::vec3(0, 4, -2);
	modelManager->InstantiateModel(programID, (char*)"res/models/TutorialModel.obj", (char*)"res/textures/BoxTex.bmp", lightPos);
	float deltaTime = (float)glfwGetTime();// float(currentTime - lastTime);
	float lastTime = (float)glfwGetTime();
	do {
		lightPos.x += 0.09f;
		currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		glUniform3f(CameraID, controls.position.x, controls.position.y, controls.position.z);
		//Compute the MVP matrix from keyboard and mouse input
		controls.ComputeMatricesFromInputs(window, deltaTime);
		entityManager->Update(deltaTime);
		modelManager->GetModel(modelManager->GetModels().size() - 1)->Move(glm::vec3(0.0001f, 0, 0));

		View = glm::lookAt(
			controls.position,						//Camera is here
			controls.position + controls.direction,	//and looks here : at the same position, plus "direction"
			controls.up								//Head is up (set to 0, -1, 0 to look upside-down)
		);

		//Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Send our transformation ot the currently bound shader, in the "MVP" uniform
		//This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(ViewID, 1, GL_FALSE, &View[0][0]);
		
		//glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);k
		glUniform3fv(glGetUniformLocation(programID, "lightPos"), 1, glm::value_ptr(lightPos));
		//std::cout << lightPos.x << ":" << lightPos.y << ":" << lightPos.z << "\n";

		glUseProgram(programID);

		if (glfwGetKey(window, GLFW_KEY_A))
			angleMultiplier.x++;
		if (glfwGetKey(window, GLFW_KEY_D))
			angleMultiplier.x--;
		if (glfwGetKey(window, GLFW_KEY_W))
			angleMultiplier.y--;
		if (glfwGetKey(window, GLFW_KEY_S))
			angleMultiplier.y++;
		modelManager->Draw(View, Projection, MatrixID, programID);
		for (unsigned int i = 0; i < modelManager->GetModels().size(); i++)
		{
			if (!modelManager->GetModel(i)->GetIsUI())
			{
				modelManager->GetModel(i)->RotateLocal(glm::vec3(2, 0, 0), 0.08f * angleMultiplier.y);
				modelManager->GetModel(i)->RotateLocal(glm::vec3(0, 2, 0), 0.08f * angleMultiplier.x);
				//modelManager->GetModel(i)->Draw(View, Projection, MatrixID, programID);
			}
		}

		lastTime = (float)glfwGetTime();
		//uiText.printText2D((char*)"25 is funnier than 24", 25, 24, 3);
		//Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	}//Check if the ESC key was pressed or the window was closed
	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	//Cleanup VBO and shader
	/*glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);*/
	glDeleteProgram(programID);
	//glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	//Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;

}
//
//Model* InstantiateModel(GLuint programID, char* modelPath, char* imagePath, glm::vec3 in_InitialPosition)
//{
//	Model* newModel = InstantiateModel(programID, modelPath, imagePath);
//	models[models.size() - 1]->Move(in_InitialPosition);
//	return newModel;
//}
//
//Model* InstantiateModel(GLuint programID, char* modelPath, char* imagePath)
//{
//	Model* newModel = new Model(programID);
//	models.push_back(newModel);
//	//(size of models array * number of buffers (eg, vertex, UV))
//	(*newModel).InitID((models.size() - 1) * 3);
//	(*newModel).LoadTexture(programID, imagePath, albedo);
//	textures.push_back(&(*newModel).albedoTexture);
//
//	UniqueModelData temp_uniqueModelData;// = new UniqueModelData;
//	temp_uniqueModelData.model = newModel;
//	temp_uniqueModelData.path = modelPath;
//
//	bool vertexDataLoaded = false;
//
//	for (int i = 0; i < uniqueModelData.size(); i++)
//	{
//		if (temp_uniqueModelData.path == uniqueModelData[i].path)
//		{//if current path equals newly created model's path
//			(*newModel).UseLoadedVertexData(*uniqueModelData[i].model);//get pointer to the currently indexed model
//			vertexDataLoaded = true;
//			break;
//		}
//	}
//
//	//if the vertex data to load hasn't been loaded into another model, load vertex data from scratch
//	if (!vertexDataLoaded)
//	{
//		(*newModel).LoadOBJ(modelPath);
//		//add to uniqueModelData so the newly loaded vertex data isn't loaded into a new model
//		uniqueModelData.push_back(temp_uniqueModelData);
//	}
//	return newModel;
//}
