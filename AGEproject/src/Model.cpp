#include "Model.h"
#include <sstream>
Model::Model()
{
}

Model::Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetUpMesh();
}


void Model::SetUpMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(BUFFERS::POSITION);
	glVertexAttribPointer(BUFFERS::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(BUFFERS::COLOR);
	glVertexAttribPointer(BUFFERS::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	// vertex normals
	glEnableVertexAttribArray(BUFFERS::NORMAL);
	glVertexAttribPointer(BUFFERS::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(BUFFERS::TEX_COORD);
	glVertexAttribPointer(BUFFERS::TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
}

void Model::Draw()
{
	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	//for (GLuint i = 0; i < this->textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i);

	//	std::string name = this->textures[i].type;
	//	std::string number = (name == "texture_diffuse") ? std::to_string(diffuseNr++) : std::to_string(specularNr++);

	//	glUniform1i(glGetUniformLocation(shader.GetId(), ("material." + name + number).c_str()), i);
	//	glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	//}
	//glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Model::CreatePlane(float spacing, unsigned int xSize, unsigned int ySize)
{
	vertices.clear();
	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			Vertex v;
			v.position = glm::vec3(i*spacing, 0.0f, j*spacing);
			v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			v.texCoords = glm::vec2(i*spacing, j*spacing);
			v.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			vertices.push_back(v);
		}
	}

	indices.clear();
	for (int row = 0; row < ySize - 1; row++)
	{
		if ((row & 1) == 0)
		{ // even rows
			for (int col = 0; col < xSize; col++)
			{
				indices.push_back(col + (row*xSize));
				indices.push_back(col + ((row + 1) * xSize));
			}
		}
		else
		{ // odd rows
			for (int col = xSize - 1; col > 0; col--)
			{
				indices.push_back(col + ((row + 1) * xSize));
				indices.push_back(col - 1 + (row * xSize));
			}
		}
	}
	SetUpMesh();
}
