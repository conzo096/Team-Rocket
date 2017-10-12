 #include "BoundingBox.h"
#include "Game.h"
#include <glm\gtx\string_cast.hpp>
#include "UserControls.h"
// Load the information onto the gpu.
void BoundingBox::SetUpBoundingBox()
{
	glGenVertexArrays(1, &bbVAO);
	glBindVertexArray(bbVAO);

	int indices[] =
	{
		7,6,3,2,0,
		6,4,7,5,
		3,1,0,5,4

		//0, 1, 2, 3, 8, // Front wall
		//4, 5, 6, 7, 8, // Back wall
		//4, 0, 6, 3, 8, // Left wall
		//1, 5, 3, 7, 8, // Right wall
		//2, 3, 6, 7, 8, // Top wall
		//0, 1, 4, 5  // Bottom wall
	};

	glm::vec3 vertices[] =
	{
		// Front wall of bounding box
		lowerLeftFront,
		glm::vec3(upperRightBack.x, lowerLeftFront.y, lowerLeftFront.z),
		glm::vec3(lowerLeftFront.x, upperRightBack.y, lowerLeftFront.z),
		glm::vec3(upperRightBack.x, upperRightBack.y, lowerLeftFront.z),
		// Back wall of bounding box
		glm::vec3(lowerLeftFront.x, lowerLeftFront.y, upperRightBack.z),
		glm::vec3(upperRightBack.x, lowerLeftFront.y, upperRightBack.z),
		glm::vec3(lowerLeftFront.x, upperRightBack.y, upperRightBack.z),
		upperRightBack
	};

	glGenVertexArrays(1, &bbVAO);
	glGenBuffers(1, &bbVBO);
	glGenBuffers(1, &bbEBO);

	glBindVertexArray(bbVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bbVBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(glm::vec3), &vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bbEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 14 * sizeof(int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);



}

void BoundingBox::Update(double deltaTime)
{
	if (CheckForMouseIntersection(RayCast(Game::Instance()->cam->GetComponent<Free_Camera>()),poi))
	{
	//	std::cout << "WOOO" << std::endl;
	}
	//else
	//	std::cout << "NOT WOO" << std::endl;
}

bool BoundingBox::CheckForMouseIntersection(RayCast ray, glm::vec3& poi)
{
	//// Get local bounds.
	glm::vec3 min = glm::vec3(lowerLeftFront.x, lowerLeftFront.y, upperRightBack.z);
	glm::vec3 max = glm::vec3(upperRightBack.x,upperRightBack.y,lowerLeftFront.z);

	// Information regarding parents position.
	glm::mat4 ModelMatrix = GetParent()->GetTransform();
	float intersection_distance;
	
	// Minumum and maximum distances.
	float tMin = 0.0f;
	float tMax = 100000.0f;


	// Find direction?
	glm::vec3 delta = GetParent()->GetPosition() - glm::dvec3(ray.origin);
	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray.direction, xaxis);

		if (fabs(f) > 0.001f)
		{ // Standard case
			float t1 = (e + min.x) / f; // Intersection with the "left" plane
			float t2 = (e + max.x) / f; // Intersection with the "right" plane
											 // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

											 // We want t1 to represent the nearest intersection, 
											 // so if it's not the case, invert t1 and t2
			if (t1>t2)
			{
				std::swap(t1, t2); // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin)
				return false;

		}
		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + min.x > 0.0f || -e + max.x < 0.0f)
				return false;
		}
	}
	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(ray.direction, yaxis);

		if (fabs(f) > 0.001f)
		{
			float t1 = (e + min.y) / f;
			float t2 = (e + max.y) / f;

			if (t1>t2)
			{ 
				std::swap(t1, t2);
			}

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + min.y > 0.0f || -e + max.y < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	// Exactly the same thing than above.
	{
		glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
		float e = glm::dot(zaxis, delta);
		float f = glm::dot(ray.direction, zaxis);

		if (fabs(f) > 0.001f)
		{
			float t1 = (e + min.z) / f;
			float t2 = (e + max.z) / f;

			if (t1>t2)
			{
				std::swap(t1, t2);
			}

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;
		}
		else
		{
			if (-e + min.z > 0.0f || -e + max.z < 0.0f)
				return false;
		}
	}
	 poi = ray.origin + ray.direction*tMin;
	 std::cout << glm::to_string(poi) << std::endl;
	return true;
}

void BoundingBox::SetUpBoundingBox(std::vector<glm::vec3> &vertices)
{
	for (glm::vec3& pos : vertices)
	{
		// Update lower-left-front corner of BB
		lowerLeftFront.x = std::min(lowerLeftFront.x, pos.x);
		lowerLeftFront.y = std::min(lowerLeftFront.y, pos.y);
		lowerLeftFront.z = std::max(lowerLeftFront.z, pos.z);
		// Update upper-right-back corner of BB
		upperRightBack.x = std::max(upperRightBack.x, pos.x);
		upperRightBack.y = std::max(upperRightBack.y, pos.y);
		upperRightBack.z = std::min(upperRightBack.z, pos.z);
	}
	lowerLeftFront.x -= 0.5;
	lowerLeftFront.y -=0.5;
	lowerLeftFront.z +=0.5;
	// Update upper-right-back corner of BB
	upperRightBack.x += 0.5;
	upperRightBack.y += 0.5;
	upperRightBack.z -= 0.5;

	SetUpBoundingBox();
};

void BoundingBox::Render()
{

	auto mvp = Game::Instance()->cam->GetComponent<Free_Camera>().GetProjection()*Game::Instance()->cam->GetComponent<Free_Camera>().GetView() * glm::mat4(GetParent()->GetTransform());
	Shader::Instance()->GetShader("Basic").Use();
	glUniformMatrix4fv(Shader::Instance()->GetShader("Basic").GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(bbVAO);
	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, 0);

}

void BoundingBox::RenderWireMesh()
{
	glBindVertexArray(bbVAO);
	glDrawElements(GL_LINE_STRIP, 14, GL_UNSIGNED_INT, 0);
}