 #include "BoundingBox.h"


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

void BoundingBox::Render()
{
	glBindVertexArray(bbVAO);
	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, 0);
}

void BoundingBox::RenderWireMesh()
{
	glBindVertexArray(bbVAO);
	glDrawElements(GL_LINE_STRIP, 14, GL_UNSIGNED_INT, 0);
}