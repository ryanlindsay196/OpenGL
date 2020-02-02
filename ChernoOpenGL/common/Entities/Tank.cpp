#include "Tank.h"
//#include "../common/Model.h"


Tank::Tank(ModelManager* in_ModelManager, GLuint in_ProgramID, EntityInstanceData* in_EntityInstanceData, EntityData* in_EntityData)
	:Entity(in_ModelManager, in_EntityData, in_ProgramID, in_EntityInstanceData)
{
	turret = InstantiateModel(in_ProgramID, glm::vec3(-2, 0, 0), glm::quat(), (char*)"res/models/TutorialModel.obj", (char*)"res/textures/SLT_Dif.png");
	body = InstantiateModel(in_ProgramID, glm::vec3(0, -2, 0), glm::quat(), (char*)"res/models/Susanne.obj", (char*)"res/textures/eggman.bmp");

	InstantiateModel(in_ProgramID, glm::vec3(0, 0, 0), glm::quat(), (char*)"res/models/TutorialModel.obj", (char*)"res/textures/BoxTex.bmp");
}


Tank::~Tank()
{
}

void Tank::Update(float gameTime)
{
	Move(glm::vec3(0, 0.0005f, 0.002f));
	Entity::Update(gameTime);
}
