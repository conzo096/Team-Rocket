#pragma once
#include <glm\common.hpp>
#include <math.h>
#include "Entity.h"
#include "RayCast.h"
#include <glm\gtx\string_cast.hpp>
class BoundingSphere : public Component
{
private:
	float radius;
	glm::vec3 poi;
public:

	BoundingSphere() : Component("BoundingSphere") {};
	~BoundingSphere() {}

	// This center will only work for symmetrical objects.
	void SetUpBoundingSphere(std::vector<glm::vec3> &vertices)
	{	
		radius = 2;
	}
	void from_json(const nlohmann::json &j) {};

	bool TestIntersection(RayCast& ray,glm::vec3& poi)
	{
		

	}


	void Update(double deltaTime) override;


	// Different render methods.
	void Render() {};
};


