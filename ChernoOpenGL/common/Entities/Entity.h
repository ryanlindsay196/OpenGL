#pragma once
#include "../ModelManager.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include "../common/EntityDataStructures.h"
#include <string>
//#include "../common/EntityManager.h"
class Entity
{
public:
	Entity(ModelManager* in_ModelManager, EntityData* in_EntityData, GLuint in_ProgramID, EntityInstanceData* in_EntityInstanceData);
	~Entity();
	virtual void Update(float gameTime);
protected:
	struct ModelData
	{
		Model* model;
		glm::vec3 positionOffset;
		glm::quat rotationOffset;
		glm::vec3 scale;
	};

	ModelManager* modelManager;
	//EntityManager* entityManager;
	float maxHealth;
	float health;
	glm::mat4 position;
	glm::quat quaternion;
	std::vector<ModelData*> modelData;

	bool isUI;

#pragma region Initialization
	Model* InstantiateModel(GLuint programID, glm::vec3 positionOffset, glm::quat rotationOffset, char* modelPath, char* albedoTexturePath);
	Model* InstantiateModel(GLuint programID, EntityData* in_EntityData, int modelDataIndex);
#pragma endregion


#pragma region Translation functions
	void Move(glm::vec3 in_Position);
	void RotateLocal(glm::vec3 angleAxis, float angle);
#pragma endregion



};

