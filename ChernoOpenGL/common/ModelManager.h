#pragma once
#include "../common/Model.h"
#include "../common/EntityDataStructures.h"
class ModelManager
{
private:
struct UniqueModelData
	{
		char* path;
		Model* model;
	};
	std::vector<UniqueModelData> uniqueModelData;

	std::vector<Model*> models;
	std::vector<Texture*> textures;
public:
	ModelManager();
	~ModelManager();
	Model* GetModel(int modelIndex);
	std::vector<Model*> GetModels();
	Model* InstantiateModel(GLuint programID, char* modelPath, char* texturePath);
	Model* InstantiateModel(GLuint programID, char* modelPath, char* texturePath, glm::vec3 in_InitialPosition);
	Model* InstantiateModel(GLuint programID, ModelData* in_ModelData);
	void Draw(glm::mat4 View, glm::mat4 Projection, int MatrixID, GLuint programID);
};

