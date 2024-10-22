#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <vector>
#include <algorithm>
#include "Entity.h"
#include "RayCast.h"
class BoundingBox : public Component
{
private:

	// Buffers for gpu.
	unsigned int bbVAO, bbVBO, bbEBO;
	// Colour of the bounding box. Used for colour picking.
	glm::vec4 colour;
	glm::vec3 poi;
public:
	// Opposite corners of the bounding box. 
	glm::vec3 lowerLeftFront = glm::vec3(999999.0f, 999999.0f, -999999.0f),
	upperRightBack = glm::vec3(-999999.0f, -999999.0f, 999999.0f);
	BoundingBox(): Component("BoundingBox") {};
	~BoundingBox() {}
	// Takes in a list of model vertices.
	void SetUpBoundingBox(std::vector<glm::vec3> &vertices);

	void SetColour(glm::vec4 col) { colour = col; }
	glm::vec4 GetColour() { return colour; }

	glm::vec3 GetPointOfIntersection(){ return poi; }

	void from_json(const nlohmann::json &j) {};

	// Load the info onto gpu.
	void SetUpBoundingBox();


	void Update(double deltaTime) override;

	bool CheckForMouseIntersection(RayCast ray,glm::vec3& poi);

	// Different render methods.
	void Render();
	void RenderWireMesh();
};