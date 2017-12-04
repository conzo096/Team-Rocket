#pragma once
#include "Renderable.h"
#include <glm/gtx/vector_angle.hpp>


class TurretRenderable : public Renderable
{
private:
	float turnSpeed;
	bool hasTarget;
	Entity* target;
public:
	TurretRenderable();
	~TurretRenderable();

	void SetTurnSpeed(float turnSpeed) { this->turnSpeed = turnSpeed; };
	void SetTarget(Entity* target) { this->target = target; hasTarget = true; };
	void ClearTarget() { target = NULL; hasTarget = false; }
	void TurnTo(double deltaTime);
	void Update(double deltaTime) override;
};