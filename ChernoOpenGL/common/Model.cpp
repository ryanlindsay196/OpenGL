#include "Model.h"
#include "VBOIndexer.h"
#include <iostream>
#include "gtc/type_ptr.hpp"

Model::Model(GLuint programID)
{
	position = glm::mat4(1);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	postPositionRotation = glm::mat4(1);
	postPositionRotationPosition = glm::mat4(1);
	LoadTexture(programID, (char*)"res/textures/normal.PNG", normal);
	quaternion = glm::quat(0, 0, 0, 1);
	scaleID = glGetUniformLocation(programID, "scale");
	//normalTextureID = glGetUniformLocation(programID, "NormalTextureSampler");
}

Model::~Model()
{

}

glm::mat4 Model::Position()
{
	return Model::position;
}

glm::vec3 Model::Rotation()
{
	glm::vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = +2.0 * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
	double cosr_cosp = +1.0 - 2.0 * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
	angles.x = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = +2.0 * (quaternion.w * quaternion.y - quaternion.z * quaternion.x);
	if (fabs(sinp) >= 1)
		angles.y = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		angles.y = asin(sinp);

	// yaw (z-axis rotation)
	double siny_cosp = +2.0 * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
	double cosy_cosp = +1.0 - 2.0 * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	angles.z = atan2(siny_cosp, cosy_cosp);

	return angles;
}

glm::quat Model::Quaternion()
{
	return Model::quaternion;
}

bool Model::GetIsUI()
{
	return Model::isUI;
}

#pragma region Vertex Get Accessors
std::vector<glm::vec3> Model::Temp_vertices()
{
	return temp_vertices;
}
std::vector<glm::vec2> Model::Temp_uvs()
{
	return temp_uvs;
}

std::vector<glm::vec3> Model::Temp_normals()
{
	return temp_normals;
}

std::vector<unsigned int> Model::M_VertexIndices()
{
	return m_VertexIndices;
}
std::vector<unsigned int> Model::M_UVIndices()
{
	return m_UVIndices;
}
std::vector<unsigned int> Model::M_NormalIndices()
{
	return m_NormalIndices;
}

std::vector<glm::vec3> Model::nM_Vertices()
{
	return m_Vertices;
}
std::vector<glm::vec2> Model::nM_UVs()
{
	return m_UVs;
}
std::vector<glm::vec3> Model::nM_Normals()
{
	return m_Normals;
}

#pragma endregion

void Model::InitID(int in_id)
{
	m_id = in_id;
}

std::vector<glm::vec3>* Model::M_Vertices()
{
	return &m_Vertices;
}
std::vector<glm::vec2>* Model::M_UVs()
{
	return &m_UVs;
}
std::vector<glm::vec3>* Model::M_Normals()
{
	return &m_Normals;
}

bool Model::LoadOBJ(const char* path)
{
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open file!\n");
		return false;
	}

	while (1) {
		char lineHeader[128];
		//read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; //EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			if (matches != 9) {
				printf("File can't be read by our simple parse: (Try exporting with other options\n");
				return false;
			}
			m_VertexIndices.push_back(vertexIndex[0]);
			m_VertexIndices.push_back(vertexIndex[1]);
			m_VertexIndices.push_back(vertexIndex[2]);
			m_UVIndices.push_back(uvIndex[0]);
			m_UVIndices.push_back(uvIndex[1]);
			m_UVIndices.push_back(uvIndex[2]);
			m_NormalIndices.push_back(normalIndex[0]);
			m_NormalIndices.push_back(normalIndex[1]);
			m_NormalIndices.push_back(normalIndex[2]);
		}
	}
	//For each vertex of each triangle
	for (unsigned int i = 0; i < m_VertexIndices.size(); i++) {
		unsigned int vertexIndex = m_VertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		m_Vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < m_UVIndices.size(); i++) {
		unsigned int UVIndex = m_UVIndices[i];
		glm::vec2 uv = temp_uvs[UVIndex - 1];
		m_UVs.push_back(uv);
	}

	for (unsigned int i = 0; i < m_NormalIndices.size(); i++) {
		unsigned int normalIndex = m_NormalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		m_Normals.push_back(normal);
	}

	InitBuffers();
	//modelPath = (char*)path;
	//glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(glm::vec3), &m_Vertices[0], GL_STATIC_DRAW);
	return true;
}

void Model::UseLoadedVertexData(Model& in_Model)
{
	temp_vertices = in_Model.Temp_vertices();
	temp_uvs = in_Model.Temp_uvs();
	temp_normals = in_Model.Temp_normals();
	//
	m_VertexIndices = in_Model.M_VertexIndices();
	m_UVIndices = in_Model.M_UVIndices();
	m_NormalIndices = in_Model.M_NormalIndices();


	m_Vertices = in_Model.nM_Vertices();
	//for (int i = 0; i < in_Model.nM_Vertices().size(); i++)
	//{
	//	glm::vec3* vertex = &in_Model.nM_Vertices()[i];
	//	m_Vertices.push_back(glm::vec3(0,0,0));
	//	&m_Vertices[m_Vertices.size() - 1] = vertex;
	//}
	m_UVs = in_Model.nM_UVs();
	m_Normals = in_Model.nM_Normals();

	InitBuffers();
}

void Model::InitBuffers()
{
	//Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	//The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(glm::vec3), &m_Vertices[0], GL_STATIC_DRAW);

	//GLuint UVBuffer;
	glGenBuffers(1, &UVbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, UVbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_UVs.size() * sizeof(glm::vec2), &m_UVs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_Normals.size() * sizeof(glm::vec3), &m_Normals[0], GL_STATIC_DRAW);

	//indexVBO(m_Vertices, m_UVs, m_Normals, m_Indices, indexed_Vertices, indexed_UVs, indexed_Normals);

	//Generate a buffer for the indices
	//glGenBuffers(1, &elementbuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned short), &m_Indices[0], GL_STATIC_DRAW);

	//glGenBuffers(1, &tangentbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
	//glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);


	//glGenBuffers(1, &bitangentbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	//glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);
}

void ComputeTangentBasis(
	//inputs
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& normals,
	//outputs
	std::vector<glm::vec3>& tangents,
	std::vector<glm::vec3>& bitangents
)
{
	for (int i = 0; i < vertices.size(); i += 3)
	{
		//Shortcuts for vertices
		glm::vec3& v0 = vertices[i + 0];
		glm::vec3& v1 = vertices[i + 1];
		glm::vec3& v2 = vertices[i + 2];

		//Shortcuts for UVs
		glm::vec2& uv0 = uvs[i + 0];
		glm::vec2& uv1 = uvs[i + 1];
		glm::vec2& uv2 = uvs[i + 2];

		//Edges of teh triangle : position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		//UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		//Set the same tangent for all three vertices of the triangle
		//They will be merged later, in vboindexer.cpp
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		//Same thing for the bitangents
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}
}

void Model::LoadTexture(GLuint programID, char* texturePath, TextureTypes textureType)
{
	if (textureType == albedo)
	{
		albedoTexture.LoadTexture(texturePath);
		albedoTextureID = glGetUniformLocation(programID, "material.diffuseTex");

		glUniform1i(albedoTextureID, 0);
	}
	else if (textureType == normal)
	{
		normalTexture.LoadTexture(texturePath);
		normalTextureID = glGetUniformLocation(programID, "material.normalTex");

		glUniform1i(normalTextureID, 1);
	}
	m_imagePath = texturePath;
}

void Model::Move(glm::vec3 movePosition)
{
	position = glm::translate(position, movePosition);
}

void Model::RotateLocal(glm::vec3 rotationAxis, float angle)
{
	rotationAxis = glm::normalize(rotationAxis);
	quaternion *= glm::angleAxis(angle, rotationAxis);
	//position = position * glm::toMat4(quaternion);
}

#pragma region Model Set accessors

void Model::SetPosition(glm::mat4 in_Position)
{
	position = in_Position;
}
void Model::SetQuaternion(glm::quat in_Quaternion)
{
	quaternion = in_Quaternion;
}

void Model::SetPostPositionRotation(glm::mat4 in_Rotation)
{
	postPositionRotation = in_Rotation;
}

void Model::SetScale(glm::vec3 in_Scale)
{
	scale = in_Scale;
}


void Model::SetPostPositionRotationPosition(glm::mat4 in_Position)
{
	postPositionRotationPosition = in_Position;
}

void Model::SetIsUI(bool in_IsUI)
{
	isUI = in_IsUI;
}

#pragma endregion

void Model::Draw(glm::mat4 View, glm::mat4 Projection, int MatrixID, GLuint programID)
{

	//set mvp matrix
	glUniform3f(scaleID, scale.x, scale.y, scale.z);
	mvp = postPositionRotationPosition * postPositionRotation * Position() * glm::toMat4(quaternion);//Remember; matrix multiplication is the other way around
	if (!isUI)
		mvp = Projection * View * mvp;
	GLuint ModelID = glGetUniformLocation(programID, "M");
	glUniform3fv(ModelID, 0, glm::value_ptr(position));
#pragma region Enable and bind vertex arrays


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                             // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	//2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVbuffer);
	glVertexAttribPointer(
		1,                           // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	//3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,							//attribute
		3,								//size
		GL_FLOAT,						//type
		GL_FALSE,						//normalized?
		0,
		(void*)0
	);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
#pragma endregion
	
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

#pragma region Texture binding
	//fix texture binding
	//glActiveTexture(*(albedoTexture.GetID()));
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, *albedoTexture.GetID());

	//glActiveTexture(*(normalTexture.GetID()));
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, *normalTexture.GetID());
	
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, albedoTextureID);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normalTextureID);
#pragma endregion

	//Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
	//glDrawElements(
	//	GL_TRIANGLES,		//mode
	//	m_Indices.size(),	//count
	//	GL_UNSIGNED_SHORT,	//type
	//	(void*)0			//element array buffer offset
	//);

	glDisableVertexAttribArray(m_id);
	glDisableVertexAttribArray(m_id + 1);
	glDisableVertexAttribArray(m_id + 2);
	quaternion = glm::quat(0, 0, 0, 1);

}