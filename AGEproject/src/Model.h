#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <string>
#include <vector>
#include "GLShader.h"
#include "assimp/Importer.hpp"
#include "assimp/PostProcess.h"
#include "assimp/Scene.h"
#include <map>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec4 color;
};

enum BUFFERS {POSITION, COLOR, NORMAL, TEX_COORD};



class Model
{
	unsigned int VAO, VBO, EBO;
	float lowestYVertex;
	float largestYVertex;
	GLenum type;
public:
	Model();
	Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void SetUpMesh();
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GLenum GetType() { return type; }
	unsigned int GetVAO() { return VAO; }
	int GetIndices() { return static_cast<int>(indices.size()); }
	void SetType(GLenum t) { type = t; }
	float GetLowestYPosition() { return lowestYVertex; }
	float GetLargestYPosition() { return largestYVertex; }
	void Draw();

	Model(const std::string& fileName);

	std::vector<glm::vec3> GetVertexPositions()
	{
		std::vector<glm::vec3> temp;
		for (Vertex vert : vertices)
			temp.push_back(vert.position);
		return temp;
	}

};