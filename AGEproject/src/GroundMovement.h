#pragma once
#include <glm/gtx/vector_angle.hpp>
#include "Movement.h"

using namespace glm;

class GroundMovement : public Movement
{
private:

protected:
	dvec3 goal;

	void from_json(const nlohmann::json &j);
public:
	GroundMovement();
	~GroundMovement();

	bool LineOfSight();
	bool Pathfind(dvec3** grid, int xSize, int ySize);
	void MoveTo(double delta);
	void TurnTo(double delta);
	void Update(double delta) override;

};