#pragma once

#define M_PI   3.14159265358979323846264338327950288

#include <glm.hpp>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include "Texture.h"
#include <math.h>

class Model 
{
private:
#pragma region
	int m_id;

	glm::mat4 mvp;
	//ROTATE AFTER MOVING POSITION
	glm::mat4 postPositionRotation;
	//MOVE AFTER POST POSITION ROTATION
	glm::mat4 postPositionRotationPosition;
	glm::vec3 scale;

	GLuint vertexbuffer;
	GLuint UVbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;

	glm::quat quaternion;

	bool isUI;
#pragma region vertex data


	std::vector<glm::vec3> m_Vertices;
	std::vector<glm::vec2> m_UVs;
	std::vector<glm::vec3> m_Normals;
	std::vector<unsigned short> m_Indices;
	
	std::vector<glm::vec3> indexed_Vertices;
	std::vector<glm::vec2> indexed_UVs;
	std::vector<glm::vec3> indexed_Normals;

	std::vector<unsigned int> m_VertexIndices, m_UVIndices, m_NormalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
#pragma endregion Model data (vertices, uvs, normals)

#pragma region Tangent buffers
	GLuint tangentbuffer;
	GLuint bitangentbuffer;
#pragma endregion


	char* m_imagePath;
	glm::mat4 position;

	void ComputeTangentBasis(
		//inputs
		std::vector<glm::vec3>& vertices,
		std::vector<glm::vec2>& uvs,
		std::vector<glm::vec3>& normals,
		//outputs
		std::vector<glm::vec3>& tangents,
		std::vector<glm::vec3>& bitangents
	);

public:
	Model(GLuint programID);
	~Model();

#pragma region Model Get Accessors
	glm::mat4 Position();
	glm::vec3 Rotation();
	glm::quat Quaternion();
#pragma endregion
#pragma region Model Set Accessors
	bool GetIsUI();
	void SetPosition(glm::mat4 in_Position);
	void SetQuaternion(glm::quat in_Quaternion);
	void SetPostPositionRotation(glm::mat4 in_Rotation);
	void SetPostPositionRotationPosition(glm::mat4 in_Position);
	void SetScale(glm::vec3 in_Scale);
	void SetIsUI(bool in_IsUI);
#pragma endregion


#pragma region Vertex Get Accessors
	std::vector<glm::vec3> Temp_vertices();
	std::vector<glm::vec2> Temp_uvs();
	std::vector<glm::vec3> Temp_normals();
	std::vector<unsigned int> M_VertexIndices();
	std::vector<unsigned int> M_UVIndices();
	std::vector<unsigned int> M_NormalIndices();
	std::vector<glm::vec3> nM_Vertices();
	std::vector<glm::vec2> nM_UVs();
	std::vector<glm::vec3> nM_Normals();
#pragma endregion


#pragma region Initialization
	void InitID(int in_id);
	//void SetTexturePath(const char* in_imagePath);
	bool LoadOBJ(const char* path);
	void UseLoadedVertexData(Model& in_Model);
	void LoadTexture(GLuint programID, char* texturePath, TextureTypes textureType);
	void InitBuffers();
#pragma endregion


#pragma region Translation
	void Move(glm::vec3 movePosition);
	void RotateLocal(glm::vec3 angleAxis, float angle);
#pragma endregion

	void Draw(glm::mat4 View, glm::mat4 Projection, int MatrixID, GLuint programID);
	
	GLuint albedoTextureID;
	Texture albedoTexture;
	GLuint normalTextureID;
	Texture normalTexture;

	GLuint scaleID;

	std::vector<glm::vec3>* M_Vertices();
	std::vector<glm::vec2>* M_UVs();
	std::vector<glm::vec3>* M_Normals();
	//char* modelPath;
};