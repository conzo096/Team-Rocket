#include "TurretRenderable.h"


TurretRenderable::TurretRenderable() : Renderable("TurretRenderable")
{
}

TurretRenderable::~TurretRenderable()
{
}


void TurretRenderable::TurnTo(double deltaTime)
{
	glm::vec3 goal = target->GetPosition();
	glm::vec3 thisPos = GetParent()->GetPosition();
	glm::vec2 targetVec = glm::vec2(thisPos.x, thisPos.z) - glm::vec2((float)goal.x, (float)goal.z);
	float distance = glm::length(targetVec);
	if (targetVec.x != 0 && targetVec.y != 0)
		targetVec = glm::normalize(targetVec);

	//glm::dvec3 f = glm::rotate(GetParent()->GetRotation(), glm::dvec3(0, 0, -1));

	glm::vec3 currentVec = glm::vec3(-GetParent()->GetTransform()[2][0], 0, -GetParent()->GetTransform()[2][2]);
	if (currentVec.x != 0 && currentVec.z != 0)
		currentVec = glm::normalize(currentVec);


	//currentVec = f;

	float angle = (glm::angle(glm::normalize(glm::vec2(currentVec.x, currentVec.z)), glm::normalize(targetVec)) / glm::pi<float>()*180.0f);

	glm::vec3 distantPoint = thisPos + (distance * 2 * currentVec);

	double determinant = ((goal.x - thisPos.x)*(distantPoint.z - thisPos.z)) - ((goal.z - thisPos.z)*(distantPoint.x - thisPos.x));

	if (determinant != 0 && !glm::isnan(angle))
	{

		if (determinant < 0)
		{
			if ((turnSpeed*deltaTime) < angle)
			{
				GetParent()->Rotate(glm::vec3(0, turnSpeed*deltaTime, 0));
			}
			else
			{
				GetParent()->Rotate(glm::vec3(0, angle, 0));
			}
		}
		else
		{
			if ((turnSpeed*deltaTime) < angle)
			{
				GetParent()->Rotate(glm::vec3(0, -turnSpeed*deltaTime, 0));
			}
			else
			{
				GetParent()->Rotate(glm::vec3(0, -angle, 0));
			}
		}
	}
}

void TurretRenderable::Update(double deltaTime)
{

}
