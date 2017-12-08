#pragma once
#include "Model.h"
#include <iostream>
#include <glm\common.hpp>
#include <vector>
class GeometryUtil
{
public:
	static Model* BuildPlane(float spacing, unsigned int xSize, unsigned int ySize)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		if (xSize == 1 && ySize == 1)
		{
			for (unsigned int i = 0; i < ySize; i++)
			{
				for (unsigned int j = 0; j < xSize; j++)
				{
					Vertex v;
					v.position = glm::vec3(j*spacing, 0.0f, i*spacing);
					v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
					v.texCoords = glm::vec2(j*spacing, i*spacing);
					v.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
					vertices.push_back(v);
				}
			}
			for (unsigned int i = 0; i < ySize; i++)
			{
				for (unsigned int j = 0; j < xSize; j++)
				{
					indices.push_back(j + (i*xSize));
					indices.push_back(j + ((i + 1) * xSize));
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < ySize; i++)
			{
				for (unsigned int j = 0; j < xSize; j++)
				{
					Vertex v;
					v.position = glm::vec3(j*spacing, 0.0f, i*spacing);
					v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
					v.texCoords = glm::vec2(j*spacing, i*spacing);
					v.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
					vertices.push_back(v);
				}
			}
			for (unsigned int i = 0; i < ySize - 1; i++)
			{
				if ((i & 1) == 0)
				{ // even rows
					for (unsigned int j = 0; j < xSize; j++)
					{
						indices.push_back(j + (i*xSize));
						indices.push_back(j + ((i + 1) * xSize));
					}
				}
				else
				{ // odd rows
					for (unsigned int j = xSize - 1; j > 0; j--)
					{
						indices.push_back(j + ((i + 1) * xSize));
						indices.push_back(j - 1 + (i * xSize));
					}
				}
			}
		}

		Model* temp = new Model(vertices, indices);
		temp->SetType(GL_TRIANGLE_STRIP);
		return temp;
	}

	static Model* BuildLine(std::vector<glm::vec3> p)
	{
		std::vector<Vertex> vertex;
		for (glm::vec3 pi : p)
		{
			Vertex v;
			v.position = pi;
			vertex.push_back(v);
		}
		std::vector<unsigned int> i;
		i.push_back(0);
		i.push_back(1);
		Model* temp = new Model(vertex, i);
		temp->SetType(GL_LINES);
		return temp;
	}




	static Model* make_plane(int width, int height)
	{
		std::vector<Vertex> vertices;
		vertices.resize(4);
		std::vector<unsigned int> indices;
		indices.resize(4);
		vertices[0].position = glm::vec3(0, 0, 0);
		vertices[1].position = glm::vec3(0, 0, height);
		vertices[2].position = glm::vec3(width, 0, height);
		vertices[3].position = glm::vec3(width, 0, 0);

		vertices[0].texCoords = glm::vec2(0, 0);
		vertices[1].texCoords = glm::vec2(0, height);
		vertices[2].texCoords = glm::vec2(width, 0);
		vertices[3].texCoords = glm::vec2(width, height);
		for (int i = 0; i < 4; i++)
		{
			vertices[i].normal = glm::vec3(0.0f, 1.0f, 0.0f);
			vertices[i].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			indices[i] = i;
		}
		
		Model* temp = new Model(vertices,indices);
		temp->SetType(GL_TRIANGLE_STRIP);
		return temp;
	}

};