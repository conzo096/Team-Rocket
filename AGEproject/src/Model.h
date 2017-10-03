#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp> 
#include <string>
#include <vector>
#include "GLShader.h"
#include "assimp/Importer.hpp"
#include "assimp/PostProcess.h"
#include "assimp/Scene.h"
#include <math.h>
#include <fstream>
#include <map>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec4 color;
};

struct Texture
{
	unsigned int id;
	std::string type;
};

enum BUFFERS {POSITION, COLOR, NORMAL, TEX_COORD};



class Model
{
private:
	unsigned int VAO, VBO, EBO;
public:
	Model();
	Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void SetUpMesh();
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	void Draw();

	Model(const std::string& fileName);

	std::vector<glm::vec3> GetVertexPositions()
	{
		std::vector<glm::vec3> temp;
		for (Vertex vert : vertices)
			temp.push_back(vert.position);
		return temp;
	}

	void CreatePlane(float spacing, unsigned int xSize, unsigned int ySize);
};