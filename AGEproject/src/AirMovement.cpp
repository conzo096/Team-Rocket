#include "AirMovement.h"

void AirMovement::from_json(const nlohmann::json & j)
{
}

AirMovement::AirMovement() : Movement("AirMovement")
{
}

AirMovement::~AirMovement()
{
}

void AirMovement::Update(double delta)
{
	if (GetParent()->GetPosition() == destination)
	{
		destination = -destination;
		destination.y = -destination.y;
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
