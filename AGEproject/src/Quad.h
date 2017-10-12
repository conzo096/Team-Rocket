#pragma once
#include "Transform.h"
#include <glm\common.hpp>
#include <vector>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
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
	Quad()
	{
		std::vector<glm::vec3> positions
		{
			glm::vec3(1.0f, 1.0f, 0.0f),
			glm::vec3(-1.0f, 1.0f, 0.0f),
			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(1.0f, -1.0f, 0.0f)
		};

		std::vector<glm::vec2> tex_coords
		{
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f)
		};

		for (int i = 0; i < 4; i++)
		{
			Vertex v;
			v.position = positions.at(i);
			v.texCoords = tex_coords.at(i);
			vertices.push_back(v);
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	}

	~Quad()
	{
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
	void Draw()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_QUADS, 0, vertices.size());
	}
};