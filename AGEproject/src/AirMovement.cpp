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
		glm::vec2 targetVec = glm::vec2((float)destination.x, (float)destination.z) - glm::vec2(thisPos.x, thisPos.z);
		float distance = glm::length(targetVec);
		if (targetVec.x != 0 && targetVec.y != 0)
			targetVec = glm::normalize(targetVec);
		float targetAngle = (atan2(targetVec.x, -targetVec.y));
		glm::vec3 currentAngleVec = (glm::eulerAngles(GetParent()->GetRotation()));
		float currentAngle = currentAngleVec.y;
		glm::vec3 currentVec = glm::vec3((float)cos(currentAngle), 0.0f, -(float)sin(currentAngle));
		if (currentVec.x != 0 && currentVec.z != 0)
			currentVec = glm::normalize(currentVec);
		float angle = glm::angle(glm::vec2(currentVec.x, currentVec.z), targetVec);

		//glm::vec3 normalA = glm::vec3((float)cos(currentAngle + (glm::pi<float>() / 2.0)), 0, (float)sin(currentAngle + (glm::pi<float>() / 2.0)));
		//glm::vec3 AB = GetParent()->GetPosition() - destination;//I think A is wrong here, need nearest point?
		//double dot = glm::dot(AB, normalA);

		glm::vec3 distantPoint = thisPos + (distance * 2 * currentVec);

		float determinant = glm::determinant(glm::mat2((distantPoint.x - thisPos.x), ((float)destination.x - thisPos.x), (distantPoint.z - thisPos.z), ((float)destination.z - thisPos.z)));

		if (glm::isnan(angle))
		{
    			printf("bugger");
		}
		if (determinant != 0)
		{
			if (determinant > 0)
			{
				//if (turnSpeed*delta < angle)
				GetParent()->Rotate(glm::vec3(0, turnSpeed*delta, 0));
				/*else
					if (!glm::isnan(angle))
						GetParent()->Rotate(glm::vec3(0, angle, 0));*/
			}
			else
			{
				//if (-turnSpeed*delta > angle)
				GetParent()->Rotate(glm::vec3(0, -turnSpeed*delta, 0));
				/*else
					if (!glm::isnan(angle))
						GetParent()->Rotate(glm::vec3(0, -angle, 0));*/
			}
		}
	}
}

void AirMovement::Update(double delta)
{
	MoveTo(delta);
	TurnTo(delta);
}
