#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <vector>
#include <algorithm>
#include "Entity.h"
class BoundingBox : public Component
{
private:
	// Buffers for gpu.
	unsigned int bbVAO, bbVBO, bbEBO;
	// Colour of the bounding box. Used for colour picking.
	glm::vec4 colour;
public:
	// Opposite corners of the bounding box. 
	glm::vec3 lowerLeftFront = glm::vec3(999999.0f, 999999.0f, -999999.0f),
	upperRightBack = glm::vec3(-999999.0f, -999999.0f, 999999.0f);
	
	BoundingBox(): Component("BoundingBox") {};
	// Takes in a list of model vertices.
	void SetUpBoundingBox(std::vector<glm::vec3>& vertices)
	{
		for (glm::vec3 pos : vertices)
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
		SetUpBoundingBox();
	};

	void SetColour(glm::vec4 col) { colour = col; }
	glm::vec4 GetColour() { return colour; }


	void from_json(const nlohmann::json &j) {};


	// Load the info onto gpu.
	void SetUpBoundingBox();

	// Different render methods.
	void Render();
	void RenderWireMesh();
};