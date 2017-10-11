#include "AirMovement.h"
#include <random>

void AirMovement::from_json(const nlohmann::json & j)
{
}

AirMovement::AirMovement() : Movement("AirMovement")
{
}

AirMovement::~AirMovement()
{
}

void AirMovement::MoveTo(double delta)
{
	if (GetParent()->GetPosition() == destination)
	{

		destination.x = rand() % 200 - 100;
		destination.z = rand() % 200 - 100;

		currentSpeed = 0.0f;

	}
	else
	{
		if (currentSpeed < speed)
			currentSpeed += speed*delta / accelerationTime;
		glm::dvec3 moveVec = destination - GetParent()->GetPosition();
		double distance = glm::length(moveVec);
		moveVec = glm::normalize(moveVec);
		moveVec = moveVec *delta * currentSpeed;
		if (distance > glm::length(moveVec))
		{
			GetParent()->Move(moveVec);
		}
		else
		{
			GetParent()->SetPosition(destination);
		}
	}
}

void AirMovement::TurnTo(double delta)
{
	if (GetParent()->GetPosition() != destination)
	{
		glm::vec3 thisPos = GetParent()->GetPosition();
		glm::vec2 targetVec = glm::vec2(destination.x, destination.z) - glm::vec2(thisPos.x, thisPos.z);
		if (targetVec.x != 0 && targetVec.y != 0)
			targetVec = glm::normalize(targetVec);
		float targetAngle = (atan2(targetVec.x, -targetVec.y) * 180 / glm::pi<float>());
		float currentAngle = ((glm::eulerAngles(GetParent()->GetRotation())).y);
		glm::vec2 currentVec = glm::vec2((float)cos(currentAngle), (float)sin(currentAngle));
		float angle = acos(glm::dot(targetVec, currentVec) / (glm::length(targetVec) * glm::length(currentVec)));

		glm::vec3 normalA = glm::vec3((float)cos(currentAngle + (glm::pi<float>() / 2.0)), 0, (float)sin(currentAngle + (glm::pi<float>() / 2.0)));
		glm::vec3 AB = GetParent()->GetPosition() - destination;//I think A is wrong here, need nearest point?
		double dot = glm::dot(AB, normalA);

		if (dot != 0)
		{
			if (dot > 0)
			{
				GetParent()->Rotate(glm::vec3(0, turnSpeed*delta, 0));
			}
			else
			{
				GetParent()->Rotate(glm::vec3(0, -turnSpeed*delta, 0));
			}
		}
	}
}

void AirMovement::Update(double delta)
{
	MoveTo(delta);
	TurnTo(delta);
}
