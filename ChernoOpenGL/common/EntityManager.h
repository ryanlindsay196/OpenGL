#pragma once
#include "Entities/Entity.h"
#include "ModelManager.h"
#include "EntityDataStructures.h"

class EntityManager
{
private:
	std::vector<Entity*> entities;
	GLuint programID;
	ModelManager* modelManager;
	SceneData sceneData;

private:
	std::vector<EntityData> entityData;
public:
	EntityManager(GLuint in_ProgramID, ModelManager* in_ModelManager);
	~EntityManager();

	void LoadEntityData(char* in_EntityPath);
	void LoadEntityInstances(char* in_EntityPath);
	void Update(float gameTime);

	void InstantiateEntity(EntityData* in_EntityData, EntityInstanceData* in_EntityInstanceData, GLuint in_ProgramID);

	glm::vec3 ParseVec3(std::string vec3String);

	//template<typename T>
	//Entity* InstantiateEntity(T entityType, glm::vec3 startPos);
};

