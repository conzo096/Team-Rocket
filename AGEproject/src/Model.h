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

enum BUFFERS {POSITION, COLOR, NORMAL, TEX_COORD};



class Model
{
private:
	unsigned int VAO, VBO, EBO;
	bool strip = false;
public:
	Model();
	Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void SetUpMesh();
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	void SetStrip(bool strip) { this->strip = strip; };
	bool GetStrip() { return strip; };
	void Draw();
	void DrawStrip();

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