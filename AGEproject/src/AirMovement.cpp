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
	
	glm::vec2 targetVec = destination.xz - GetParent()->GetPosition().xz;
	targetVec = glm::normalize(targetVec);
	float targetAngle = (atan2(targetVec.x, -targetVec.y) * 180 / glm::pi<float>());
	float angleDifference = targetAngle - (glm::eulerAngles(GetParent()->GetRotation())).y;
	angleDifference += 180;
	float rawAngle = angleDifference - floor(angleDifference / 360) * 360;
	rawAngle -= 180;

	if (rawAngle != 0)
	{
		if (rawAngle > 0)
		{
			GetParent()->Rotate(glm::vec3(0, turnSpeed*delta, 0));
		}
		else
		{
			GetParent()->Rotate(glm::vec3(0, -turnSpeed*delta, 0));
		}
	}

}

void AirMovement::Update(double delta)
{
	MoveTo(delta);
	TurnTo(delta);
}
