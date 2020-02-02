#include "Entity.h"

Entity::Entity(ModelManager* in_ModelManager, EntityData* in_EntityData, GLuint in_ProgramID, EntityInstanceData* in_EntityInstanceData):
	modelManager(in_ModelManager)
{
	position = glm::mat4(1);
	position = glm::translate(position, in_EntityInstanceData->position);

	for (unsigned int i = 0; i < in_EntityData->modelData.size(); i++)
	{
		ModelData* tempModelData = new ModelData();
		tempModelData->model = InstantiateModel(in_ProgramID, in_EntityData->modelData[i].positionOffset, in_EntityData->modelData[i].rotationOffset, (char*)in_EntityData->modelData[i].modelPath.c_str(), (char*)in_EntityData->modelData[i].diffuseTexturePath.c_str());
		tempModelData->positionOffset = in_EntityData->modelData[i].positionOffset;
		tempModelData->rotationOffset = in_EntityData->modelData[i].rotationOffset;
		tempModelData->scale = in_EntityData->modelData[i].scale;
		tempModelData->model->SetIsUI(in_EntityData->isUI);
		tempModelData->model->SetScale(tempModelData->scale);
		//modelData.push_back(tempModelData);
	}
	//quaternion = glm::quat(0, 0, 0, 1);
	quaternion = in_EntityInstanceData->quaternion;

	isUI = in_EntityData->isUI;
}


Entity::~Entity()
{
}

void Entity::Update(float gameTime)
{
	RotateLocal(glm::vec3(2, 0, 0), 0.002f);
	for (unsigned int i = 0; i < modelData.size(); i++)
	{//make sure to update
		//rotate models around the entity point (the center)
		//Reset position to origin
		modelData[i]->model->SetPosition(glm::mat4(1));
		//Local Rotation
		modelData[i]->model->SetPosition(modelData[i]->model->Position() * glm::toMat4(modelData[i]->rotationOffset));
		//Move to positionOffset to rotate around origin
		modelData[i]->model->Move(modelData[i]->positionOffset);
		//Rotation relative to entity
		modelData[i]->model->SetPostPositionRotation(glm::toMat4(quaternion));

		//move the model by the entity's position
		modelData[i]->model->SetPostPositionRotationPosition(position);
	}
}

//DEPRECIATED
Model* Entity::InstantiateModel(GLuint programID, glm::vec3 positionOffset, glm::quat rotationOffset, char* modelPath, char* albedoTexturePath)
{
	//instantiate model data and apply transformation offset

	//add model data to internal list of model data for this entity
	ModelData* tempModelData = new ModelData();
	modelData.push_back(tempModelData);
	//instantiate model and record it's address
	tempModelData->model = modelManager->InstantiateModel(programID, modelPath, albedoTexturePath);
	//set model's position based on this entity's position
	tempModelData->model->SetPosition(position);
	//set position and rotation offsets based on arguments in this function
	tempModelData->positionOffset = positionOffset;
	tempModelData->rotationOffset = rotationOffset;

	tempModelData->model->Move(positionOffset);
	tempModelData->model->SetQuaternion(rotationOffset);

	return modelManager->GetModel(modelManager->GetModels().size() - 1);
}

Model* Entity::InstantiateModel(GLuint programID, EntityData* in_EntityData, int modelDataIndex)
{
	//instantiate model data and apply transformation offset

//add model data to internal list of model data for this entity
	ModelData* tempModelData = new ModelData();
	modelData.push_back(tempModelData);
	//instantiate model and record it's address
	//tempModelData->model = modelManager->InstantiateModel(programID, (char*)in_EntityData->modelData[modelDataIndex].modelPath.c_str(), (char*)in_EntityData->modelData[modelDataIndex].diffuseTexturePath.c_str());
	tempModelData->model = modelManager->InstantiateModel(programID, &(in_EntityData->modelData[modelDataIndex]));
	//set model's position based on this entity's position
	tempModelData->model->SetPosition(position);
	//set position and rotation offsets based on arguments in this function
	tempModelData->positionOffset = in_EntityData->modelData[modelDataIndex].positionOffset;
	tempModelData->rotationOffset = in_EntityData->modelData[modelDataIndex].rotationOffset;

	tempModelData->model->Move(in_EntityData->modelData[modelDataIndex].positionOffset);
	tempModelData->model->SetQuaternion(in_EntityData->modelData[modelDataIndex].rotationOffset);

	return modelManager->GetModel(modelManager->GetModels().size() - 1);
}

void Entity::Move(glm::vec3 in_Position)
{
	position = glm::translate(position, in_Position);
}

void Entity::RotateLocal(glm::vec3 angleAxis, float angle)
{
	angleAxis = glm::normalize(angleAxis);
	quaternion *= glm::angleAxis(angle, angleAxis);
}