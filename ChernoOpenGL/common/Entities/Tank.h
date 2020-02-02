#pragma once
#include "Entity.h"
#include "../Model.h"
class Tank : public Entity
{
	Model* body;
	Model* turret;
public:
	Tank(ModelManager* in_ModelManager, GLuint in_ProgramID, EntityInstanceData* in_EntityInstanceData, EntityData* in_EntityData);
	~Tank();

	void Update(float gameTime);
};

