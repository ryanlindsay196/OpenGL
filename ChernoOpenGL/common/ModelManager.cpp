#include "ModelManager.h"



ModelManager::ModelManager()
{
}


ModelManager::~ModelManager()
{
}

Model* ModelManager::GetModel(int modelIndex)
{
	return models[modelIndex];
}

std::vector<Model*> ModelManager::GetModels()
{
	return models;
}

//Instantiates with albedo/diffuse texture and position, but no rotation or normal texture
Model* ModelManager::InstantiateModel(GLuint programID, char* modelPath, char* imagePath, glm::vec3 in_InitialPosition)
{
	Model* newModel = InstantiateModel(programID, modelPath, imagePath);
	models[models.size() - 1]->Move(in_InitialPosition);
	return newModel;
}
//Doesn't instantiate with position or rotation
Model* ModelManager::InstantiateModel(GLuint programID, char* modelPath, char* imagePath)
{
	Model* newModel = new Model(programID);
	models.push_back(newModel);
	//(size of models array * number of buffers (eg, vertex, UV))
	(*newModel).InitID((models.size() - 1) * 3);
	(*newModel).LoadTexture(programID, imagePath, albedo);
	textures.push_back(&(*newModel).albedoTexture);

	UniqueModelData temp_uniqueModelData;// = new UniqueModelData;
	temp_uniqueModelData.model = newModel;
	temp_uniqueModelData.path = modelPath;

	bool vertexDataLoaded = false;

	for (unsigned int i = 0; i < uniqueModelData.size(); i++)
	{
		if (temp_uniqueModelData.path == uniqueModelData[i].path)
		{//if current path equals newly created model's path
			(*newModel).UseLoadedVertexData(*uniqueModelData[i].model);//get pointer to the currently indexed model
			vertexDataLoaded = true;
			break;
		}
	}

	//if the vertex data to load hasn't been loaded into another model, load vertex data from scratch
	if (!vertexDataLoaded)
	{
		(*newModel).LoadOBJ(modelPath);
		//add to uniqueModelData so the newly loaded vertex data isn't loaded into a new model
		uniqueModelData.push_back(temp_uniqueModelData);
	}
	return newModel;
}
//Instantiates with position, rotation, and all textures
Model* ModelManager::InstantiateModel(GLuint programID, ModelData* in_ModelData)
{
	Model* newModel = new Model(programID);
	newModel = InstantiateModel(programID, (char*)in_ModelData->modelPath.c_str(), (char*)in_ModelData->diffuseTexturePath.c_str());
	//Loads the normal texture
	models[models.size() - 1]->LoadTexture(programID, (char*)in_ModelData->normalTexturePath.c_str(), normal);
	textures.push_back(&(*newModel).normalTexture);

	models[models.size() - 1]->Move(in_ModelData->positionOffset);
	models[models.size() - 1]->SetQuaternion(in_ModelData->rotationOffset);

	return newModel;
}
void ModelManager::Draw(glm::mat4 View, glm::mat4 Projection, int MatrixID, GLuint programID)
{
	for (unsigned int i = 0; i < models.size(); i++)
	{
		models[i]->Draw(View, Projection, MatrixID, programID);
	}
}