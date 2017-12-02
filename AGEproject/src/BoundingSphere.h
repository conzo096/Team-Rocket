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
	glm::vec3 center;
	glm::vec3 poi;
public:

	BoundingSphere() : Component("BoundingSphere") {};
	~BoundingSphere(){}

	// This center will only work for symmetrical objects.
	void SetUpBoundingSphere(std::vector<glm::vec3> &vertices)
	{	
		glm::vec3 modelCenter = glm::vec3(0);

		for(glm::vec3 &v : vertices)
		{
			
			modelCenter += v;
		}

		modelCenter /= vertices.size();

		// Now we know the center point, we can compute the model radius  
		// by examining the radius of each mesh bounding sphere.  
		radius = 0;

		for(glm::vec3 &v : vertices)
		{
			float length = float((modelCenter - v).length());
			radius = std::max(radius,length);
		}
		radius += 2;
	}
	

	glm::vec3 GetCenter() { return center; }
	void SetCenter(glm::vec3 c) { center = c; }

	float GetRadius() { return radius; }

	void SetUpBoundingSphere(float rad,glm::vec3 c)
	{
		radius = rad;
		center = c;
	}
	
	void from_json(const nlohmann::json &j) {};

	bool TestIntersection(RayCast& ray, glm::vec3& poi = glm::vec3(0))
	{
		glm::vec3 origin_position = center - glm::vec3(ray.origin);
		double b = glm::dot(origin_position,ray.direction);
		double determinant = b * b - glm::dot(origin_position,origin_position) + radius * radius;
		if (determinant < 0)
		{
			return false;
		}
		else
		{
			determinant = sqrt(determinant);
		}
		double t = b - determinant;
		if (t > 1e-4)
		{
			return true;
		}
		else
		{
			t = b + determinant;
			if (t > 1e-4)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	bool DetectSphereSphereIntersection(BoundingSphere &other)
	{
		float val = glm::distance(center, other.center);
		float r = radius + other.radius;
		if (val <= r)
			return true;
		return false;
				

	}


	void Update(double deltaTime) override;


	// Different render methods.
	void Render(){}
};


