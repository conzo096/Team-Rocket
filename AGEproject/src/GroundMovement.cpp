#include "GroundMovement.h"
#include <random>
#include <queue>

void GroundMovement::from_json(const nlohmann::json & j)
{
}

GroundMovement::GroundMovement() : Movement("GroundMovement")
{
}

GroundMovement::~GroundMovement()
{
}

bool GroundMovement::LineOfSight()
{
	return false;
}

bool GroundMovement::Pathfind(dvec3 ** grid, int xSize, int ySize)
{
	std::queue<dvec3> open, closed;
	float pathDist = 0;
	dvec3 parent = this->GetParent()->GetPosition();
	open.emplace(this->GetParent()->GetPosition(), glm::length(goal - this->GetParent()->GetPosition()));
	while (open.size != 0)
	{
		dvec3 s = open.pop;
		if (s == goal)
			return true;
		//closed = closedU{s};
	}
	return false;
}

void GroundMovement::MoveTo(double delta)
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
		dvec3 moveVec = destination - GetParent()->GetPosition();
		double distance = glm::length(moveVec);
		moveVec = glm::normalize(moveVec);
		moveVec = moveVec *delta * currentSpeed;
		if (distance > length(moveVec))
		{
			GetParent()->Move(moveVec);
		}
		else
		{
			GetParent()->SetPosition(destination);
		}
	}
}

void GroundMovement::TurnTo(double delta)
{
	if (GetParent()->GetPosition() != destination)
	{
		vec3 thisPos = GetParent()->GetPosition();
		vec2 targetVec = vec2(thisPos.x, thisPos.z) - vec2((float)destination.x, (float)destination.z);
		float distance = length(targetVec);
		if (targetVec.x != 0 && targetVec.y != 0)
			targetVec = glm::normalize(targetVec);

		//glm::dvec3 f = glm::rotate(GetParent()->GetRotation(), glm::dvec3(0, 0, -1));

		vec3 currentVec = vec3(-GetParent()->GetTransform()[2][0], 0, -GetParent()->GetTransform()[2][2]);
		if (currentVec.x != 0 && currentVec.z != 0)
			currentVec = normalize(currentVec);


		//currentVec = f;

		float angle = (glm::angle(normalize(vec2(currentVec.x, currentVec.z)), normalize(targetVec)) / pi<float>()*180.0f);

		vec3 distantPoint = thisPos + (distance * 2 * currentVec);

		float determinant = ((destination.x - thisPos.x)*(distantPoint.z - thisPos.z)) - ((destination.z - thisPos.z)*(distantPoint.x - thisPos.x));

		if (determinant != 0 && !glm::isnan(angle))
		{

			if (determinant < 0)
			{
				if ((turnSpeed*delta) < angle)
				{
					GetParent()->Rotate(vec3(0, turnSpeed*delta, 0));
				}
				else
				{
					GetParent()->Rotate(vec3(0, angle, 0));
				}
			}
			else
			{
				if ((turnSpeed*delta) < angle)
				{
					GetParent()->Rotate(vec3(0, -turnSpeed*delta, 0));
				}
				else
				{
					GetParent()->Rotate(vec3(0, -angle, 0));
				}
			}
		}
	}
}

void GroundMovement::Update(double delta)
{
	MoveTo(delta);
	TurnTo(delta);
}
