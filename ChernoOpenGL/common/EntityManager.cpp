#include "EntityManager.h"
#include <fstream>
#include <iostream>
#include "Entities/Entity.h"
#include "Entities/Tank.h"
#include "gtx/quaternion.hpp"
#include <string>
using namespace std;

EntityManager::EntityManager(GLuint in_ProgramID, ModelManager* in_ModelManager) :
	programID(in_ProgramID), modelManager(in_ModelManager)
{
	//modelManager = in_ModelManager;
	//programID = in_ProgramID;
	LoadEntityData((char*)"res/loadFiles/EntityData.txt");
	LoadEntityInstances((char*)"res/loadFiles/EntityInstances.txt");
	//Tank* tank = new Tank(modelManager, programID, glm::vec3(0, 4, -2));
	//entities.push_back(tank);
	//Tank* tank2 = new Tank(modelManager, programID, glm::vec3(3, 6, -3));
	//entities.push_back(tank2);
}


EntityManager::~EntityManager()
{
}
#pragma region Load entity data and instances
void EntityManager::LoadEntityData(char* in_FilePath)
{
	//load entities from file
	ifstream entityDataFile;
	string currentLine;
	entityDataFile.open(in_FilePath);
	if (entityDataFile)
	{//ENTITY DATA
		while(getline(entityDataFile, currentLine))
		//while (getline(entityDataFile, currentLine))
		{
			if (currentLine.length() <= 0 || currentLine.at(0) == '#')
				continue;
			string header = currentLine;
			header = strtok((char*)header.c_str(), ":");
			header = header.substr(header.find_first_of('[') + 1);

			string footer = currentLine;
			footer = footer.substr(currentLine.find_first_of(':') + 1);
			footer = strtok((char*)footer.c_str(), "]");
			if (header == "Entity")
			{
				entityData.push_back(EntityData());
				entityData[entityData.size() - 1].entityName = footer;

				entityData[entityData.size() - 1].isUI = false;
			}
			else if (header == "Model")
			{
				entityData[entityData.size() - 1].modelData.push_back(ModelData());
				entityData[entityData.size() - 1].modelData[entityData[entityData.size() - 1].modelData.size() - 1].modelPath = (char*)footer.c_str();

			}
			else if (header == "PositionOffset")
			{
				footer = strtok((char*)footer.c_str(), ")");
				footer = footer.substr(footer.find_first_of('(') + 1);

				#pragma region Set vecX, vecY & vecZ
				string vecX, vecY, vecZ;
				vecX = vecY = vecZ = footer;

				vecX = strtok((char*)vecX.c_str(), ",");

				vecY = vecY.substr(vecY.find_first_of(',') + 1);
				vecY = strtok((char*)vecY.c_str(), ",");

				vecZ = vecZ.substr(vecZ.find_last_of(',') + 1);
				#pragma endregion

				entityData[entityData.size() - 1].modelData[entityData[entityData.size() - 1].modelData.size() - 1].positionOffset = glm::vec3(stof(vecX), stof(vecY), stof(vecZ));
			}
			else if (header == "RotationOffset")
			{
				#pragma region Set angle axes


				string angleAxisX, angleAxisY, angleAxisZ, angle;
				angleAxisX = angleAxisY = angleAxisZ = angle = footer;

				angleAxisX = strtok((char*)angleAxisX.c_str(), ",");
				angleAxisX = angleAxisX.substr(angleAxisX.find_first_of('(') + 1);

				angleAxisY = angleAxisY.substr(angleAxisY.find_first_of(',') + 1);
				angleAxisY = strtok((char*)angleAxisY.c_str(), ",");

				angleAxisZ = angleAxisZ.substr(angleAxisZ.find_first_of(',')).substr(angleAxisZ.find_first_of(',') + 1);
				angleAxisZ = strtok((char*)angleAxisZ.c_str(), ")");

				angle = angle.substr(angle.find_last_of(',') + 1);

				glm::vec3 angleAxis = glm::vec3(stof(angleAxisX), stof(angleAxisY), stof(angleAxisZ));
				angleAxis = glm::normalize(angleAxis);
				#pragma endregion

				entityData[entityData.size() - 1].modelData[entityData[entityData.size() - 1].modelData.size() - 1].rotationOffset = glm::angleAxis(stof(angle), angleAxis);
			}
			else if (header == "Scale")
			{
				footer = strtok((char*)footer.c_str(), ")");
				footer = footer.substr(footer.find_first_of('(') + 1);

				#pragma region Set vecX, vecY & vecZ
				string vecX, vecY, vecZ;
				vecX = vecY = vecZ = footer;

				vecX = strtok((char*)vecX.c_str(), ",");

				vecY = vecY.substr(vecY.find_first_of(',') + 1);
				vecY = strtok((char*)vecY.c_str(), ",");

				vecZ = vecZ.substr(vecZ.find_last_of(',') + 1);
				#pragma endregion

				entityData[entityData.size() - 1].modelData[entityData[entityData.size() - 1].modelData.size() - 1].scale = glm::vec3(stof(vecX), stof(vecY), stof(vecZ));
			}
			else if (header == "DiffuseTexture")
			{
				entityData[entityData.size() - 1].modelData[entityData[entityData.size() - 1].modelData.size() - 1].diffuseTexturePath = (char*)footer.c_str();
			}
			else if (header == "NormalTexture")
			{
				entityData[entityData.size() - 1].modelData[entityData[entityData.size() - 1].modelData.size() - 1].normalTexturePath = (char*)footer.c_str();
			}
			else if (header == "UI")
			{
				if (footer == "True")
				{
					entityData[entityData.size() - 1].isUI = true;
				}
			}
		}
	}
	else
	{
		std::cout << "ERROR::EntityManager::LoadEntityData::Entity data file could not be loaded!\n";
	}
	entityDataFile.close();
}

void EntityManager::LoadEntityInstances(char* in_EntityPath)
{

	ifstream entityInstancesFile;
	string currentLine;
	entityInstancesFile.open(in_EntityPath);
	EntityInstanceData currentEntityInstanceData;
	if (entityInstancesFile)
	{
		int currentEntityDataID = 0;
		while (getline(entityInstancesFile, currentLine))
		{
			if (currentLine.size() <= 0 || currentLine.at(0) == '#')
				continue;

			string header = currentLine;
			header = strtok((char*)header.c_str(), ":");
			header = header.substr(header.find_first_of('[') + 1);

			string footer = currentLine;
			footer = footer.substr(currentLine.find_first_of(':') + 1);
			footer = strtok((char*)footer.c_str(), "]");

			if (header == "CameraPos")
			{
				sceneData.cameraPos = ParseVec3(footer);
			}
			else if (header == "CameraRotation")
			{
				sceneData.cameraRotation = ParseVec3(footer);
			}
			else if (header == "CameraLock")
			{
				//sceneData.cameraLockAxes.lockX = 
				//sceneData.cameraLockAxes.lockY = 
				//sceneData.cameraLockAxes.lockZ = 
			}
			if (header == "Entity")
			{
				for (unsigned int i = 0; i < entityData.size(); i++)
				{
					if (footer == entityData[i].entityName)
					{
						currentEntityInstanceData = EntityInstanceData();
						currentEntityDataID = i;
					}
				}
			}
			else if (header == "/Entity")
			{
				InstantiateEntity(&entityData[currentEntityDataID], &currentEntityInstanceData, programID);
			}
			else if (header == "Position")
			{

				footer = strtok((char*)footer.c_str(), ")");
				footer = footer.substr(footer.find_first_of('(') + 1);
				string vecX, vecY, vecZ;
				vecX = vecY = vecZ = footer;

				vecX = strtok((char*)vecX.c_str(), ",");

				vecY = vecY.substr(vecY.find_first_of(',') + 1);
				vecY = strtok((char*)vecY.c_str(), ",");

				vecZ = vecZ.substr(vecZ.find_last_of(',') + 1);

				currentEntityInstanceData.position = glm::vec3(stof(vecX), stof(vecY), stof(vecZ));
				//entityData[entityData.size() - 1].modelData[entityData[entityData.size() - 1].modelData.size() - 1].positionOffset = glm::vec3(stof(vecX), stof(vecY), stof(vecZ));
			}
			else if (header == "Rotation")
			{

				string angleAxisX, angleAxisY, angleAxisZ, angle;
				angleAxisX = angleAxisY = angleAxisZ = angle = footer;

				angleAxisX = strtok((char*)angleAxisX.c_str(), ",");
				angleAxisX = angleAxisX.substr(angleAxisX.find_first_of('(') + 1);

				angleAxisY = angleAxisY.substr(angleAxisY.find_first_of(',') + 1);
				angleAxisY = strtok((char*)angleAxisY.c_str(), ",");

				angleAxisZ = angleAxisZ.substr(angleAxisZ.find_first_of(',')).substr(angleAxisZ.find_first_of(',') + 1);
				angleAxisZ = strtok((char*)angleAxisZ.c_str(), ")");

				angle = angle.substr(angle.find_last_of(',') + 1);

				glm::vec3 angleAxis = glm::vec3(stof(angleAxisX), stof(angleAxisY), stof(angleAxisZ));
				angleAxis = glm::normalize(angleAxis);
				currentEntityInstanceData.quaternion = glm::angleAxis(stof(angle), angleAxis);
			}
		}
	}
	else
	{
		std::cout << "ERROR::EntityManager::LoadEntityInstances::Entity instances file could not be loaded";
	}

	entityInstancesFile.close();
}
#pragma endregion

void EntityManager::Update(float gameTime)
{
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		//dynamically cast to the correct data type
		(entities[i])->Update(gameTime);
	}
}

void EntityManager::InstantiateEntity(EntityData* in_EntityData, EntityInstanceData* in_EntityInstanceData, GLuint in_ProgramID)
{
	//Tank* tank = new Tank(modelManager, programID, glm::vec3(0, 4, -2));
	Entity* entity = new Entity(modelManager, in_EntityData, in_ProgramID, in_EntityInstanceData);
	entities.push_back(entity);
}

glm::vec3 EntityManager::ParseVec3(string vec3String)
{
	glm::vec3 parsedVec3 = glm::vec3(0, 0, 0);
	return parsedVec3;
}

//template <typename T>
//Entity* EntityManager::InstantiateEntity(T entityType, glm::vec3 startPos)
//{
	//Entity* entity = new T();
	//entities.push_back(entity);
//}