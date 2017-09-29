#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <vector>
#include <algorithm>
class BoundingBox
{
public:
	// Bounding box data - two opposite corners of model's bounding box
	glm::vec3 lowerLeftFront = glm::vec3(999999.0f, 999999.0f, -999999.0f),
	upperRightBack = glm::vec3(-999999.0f, -999999.0f, 999999.0f);
	unsigned int bbVAO,bbVBO,bbEBO;
	
	BoundingBox() {};
	BoundingBox(std::vector<glm::vec3>& vertices);

	void SetUpBoundingBox();
	void Render();
	void RenderWireMesh();
};