#include "Quad.h"
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>
#include "UserControls.h"

Quad::Quad()
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

Quad::Quad(glm::vec2 bottomLeft, glm::vec2 topRight)
{
	std::vector<glm::vec3> positions
	{
		glm::vec3(topRight.x, topRight.y, 0.0f),
		glm::vec3(bottomLeft.x, topRight.y, 0.0f),
		glm::vec3(bottomLeft.x, bottomLeft.y, 0.0f),
		glm::vec3(topRight.x, bottomLeft.y, 0.0f)
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

}

Quad::~Quad()
{
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Quad::SetOpenGL()
{
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

bool Quad::RayIntersection(RayCast ray)
{
	glm::vec3 temp[4];
	for (int i = 0; i < 4; i++)
	{
		temp[i] = glm::vec3(GetTransform()*glm::dvec4(vertices[i].position, 1));
	}

	glm::vec3 normal = glm::cross(temp[1] - temp[0], temp[0] - temp[2]);
	normal = glm::normalize(normal);
	// Distance from offset.
	float offset = (float)GetPosition().length();


	//// Get local bounds.
	glm::vec3 min = glm::vec3(-1, -1, -1);
	glm::vec3 max = glm::vec3(1, 1, 1);

	// Information regarding parents position.
	glm::mat4 ModelMatrix = GetTransform();

	// Minumum and maximum distances.
	float tMin = 0.0f;
	float tMax = 100000.0f;


	// Find direction?
	glm::vec3 delta = GetPosition() - glm::dvec3(ray.origin);
	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray.direction, xaxis);

		if (fabs(f) > 0.001f)
		{ // Standard case
			float t1 = (e + min.x) / f; // Intersection with the "left" plane
			float t2 = (e + max.x) / f; // Intersection with the "right" plane
										// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

										// We want t1 to represent the nearest intersection, 
										// so if it's not the case, invert t1 and t2
			if (t1>t2)
			{
				std::swap(t1, t2); // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			//			if (tMax < tMin)
			//				return false;

		}
		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + min.x > 0.0f || -e + max.x < 0.0f)
				return false;
		}
	}
	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(ray.direction, yaxis);

		if (fabs(f) > 0.001f)
		{
			float t1 = (e + min.y) / f;
			float t2 = (e + max.y) / f;

			if (t1>t2)
			{
				std::swap(t1, t2);
			}

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + min.y > 0.0f || -e + max.y < 0.0f)
				return false;
		}
	}
	return true;
}

bool Quad::IsMouseInBounds()
{
	int w, h;
	glm::vec2 mousePos = UserControls::Get().GetMousePos();
	glfwGetWindowSize(GameEngine::Get().GetWindow(), &w, &h);
	//glfwGetFramebufferSize(GameEngine::Get().GetWindow(), &w, &h);

	// Check mouse x is within % range of quad.
	mousePos.x /= w;
	mousePos.x = (mousePos.x * 2.0f) - 1.0f;

	// Here's the problem
	float bottomLeftX = vertices[2].position.x;
	float topRightX = vertices[0].position.x;

	// Check mouse y is within % range of quad.
	mousePos.y /= h;
	mousePos.y = 1.0f - (mousePos.y * 2.0f);

	// Here's the problem
	float bottomLeftY = vertices[2].position.y;
	float topRightY = vertices[0].position.y;

	if ((mousePos.x >= bottomLeftX) && (mousePos.x <= topRightX) &&
		(mousePos.y >= bottomLeftY) && (mousePos.y <= topRightY))
	{
		return true;
	}
	else
		return false;
}

void Quad::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_QUADS, 0, (GLsizei)vertices.size());
}