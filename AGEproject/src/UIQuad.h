#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <gl\glew.h>
#include "ResourceHandler.h"

class UIQuad
{

private: 
	// What shader the quad uses.
	unsigned int shader;
	// What the quad represents.
	//char* text;
	std::string text;

	// Render info.
	unsigned int Text2DVertexBufferID;
	unsigned int Text2DUVBufferID;
	unsigned int Text2DUniformID;

	// Dimensions of quad.
	int xPos, yPos, size;
public:

	UIQuad()
	{
		xPos = 10;
		yPos = 500;
		size = 60;
		text = "Insert Text";
		shader = ResourceHandler::Get().GetShader("Font")->GetId();
		Text2DUniformID = glGetUniformLocation(shader, "myTextureSampler");
		// Initialize VBO
		glGenBuffers(1, &Text2DVertexBufferID);
		glGenBuffers(1, &Text2DUVBufferID);
	}

	void Render();

	void SetText(char* newText)
	{
		text = newText;
	}
	void SetText(const char* newText)
	{
		text = newText;
	}

	void SetX(int x)
	{
		xPos = x;
	}
	void SetY(int y)
	{
		yPos = y;
	}
	void SetSize(int s)
	{
		size = s;
	}

};

