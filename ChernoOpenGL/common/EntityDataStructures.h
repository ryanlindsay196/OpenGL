#pragma once
#include <string>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/quaternion.hpp"

struct CameraLockAxes
{
	bool lockX;
	bool lockY;
	bool lockZ;
};

struct ModelData
{
	std::string modelPath;
	std::string diffuseTexturePath;
	std::string normalTexturePath;

	glm::vec3 positionOffset;
	glm::quat rotationOffset;
	glm::vec3 scale;
};

struct EntityData
{
	std::string entityName;
	std::vector<ModelData> modelData;
	bool isUI;
};

struct EntityInstanceData
{
	glm::vec3 position;
	glm::quat quaternion;
};

struct SceneData
{
	glm::vec3 cameraPos;
	glm::vec3 cameraRotation;

	CameraLockAxes cameraLockAxes;
};