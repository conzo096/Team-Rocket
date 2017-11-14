#pragma once
#include <vector>
#include <glm\common.hpp>
#include "Transform.h"
#include "RayCast.h"

class Quad : public Transform
{
public:
	unsigned int VBO;
	unsigned int VAO;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texCoords;
	};
	std::vector<Vertex> vertices;
	//create the quad here
	Quad();

	Quad(glm::vec2 bottomLeft, glm::vec2 topRight);

	~Quad();

	void SetOpenGL();
	bool RayIntersection(RayCast ray);

	bool IsMouseInBounds();
	
	void Draw();
};