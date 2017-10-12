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
public:

	BoundingSphere() : Component("BoundingSphere") {};
	~BoundingSphere() {}

	// This center will only work for symmetrical objects.
	void SetUpBoundingSphere(std::vector<glm::vec3> &vertices)
	{
		// centre of model
		glm::vec3 center;
		for (glm::vec3& v : vertices)
		{
			center += v;
		}
		center /= 2;
	
		// Find longest length from center.
		for (glm::vec3& v : vertices)
		{
			if ((center - v).length() > radius)
				radius = (center - v).length();
		}
	
	}
	void from_json(const nlohmann::json &j) {};

	bool TestIntersection(RayCast& ray)
	{
		glm::vec3 pos = GetParent()->GetPosition() - glm::dvec3(ray.origin);			
		float rSquared = radius*radius;
		float pd = glm::dot(pos, ray.direction);
		if (pd > 0 || glm::dot(pos, pos) < rSquared)
			return false;
		
		// Flatten p into the plane passing through c perpendicular to the ray.
		// This gives the closest approach of the ray to the center.
		glm::vec3 a = pos - pd * ray.direction;
		
		float aSquared = dot(a, a);
		
		// Closest approach is outside the sphere.
		if (aSquared > rSquared)
			return false;
		
		// Calculate distance from plane where ray enters/exits the sphere.    
		float h = sqrt(rSquared - aSquared);
		
		// Calculate intersection point relative to sphere center.
		glm::vec3 i = a - h * ray.direction;
		
		glm::vec3 intersection = GetParent()->GetPosition() + glm::dvec3(i);
		
		std::cout << glm::to_string(intersection) << std::endl;
		return true;
	}


	void Update(double deltaTime) override;


	// Different render methods.
	void Render() {};
};


