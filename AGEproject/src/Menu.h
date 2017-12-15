// Generic menu structure. 
#pragma once
#include <vector>
#include <string>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc/matrix_transform.hpp>
#include "GLShader.h"
#include "Texture.h"
#include "Quad.h"
#include "Singleton.h"
struct Label
{
	// Texture of the label.
	unsigned int texture;
	// Quad that the label is rendered on.
	Quad renderTarget;
};

struct Button
{
	// Texture of the button.
	unsigned int texture;
	// What action the button does.
	int action;
	// Quad that the button is rendered on.
	Quad renderTarget;
};

class Menu //: public Singleton<Menu>
{
protected:
	bool mouseButtonHeld;
	bool selectionMade;

	// small timer for controller.
	int cooldown = 6;
	// frames since last call.
	int timeElapsed = 0;

public:
	Menu()
	{
		mouseButtonHeld = false;
		selectionMade = false;
	}
	//Menu(std::vector<char *> textureLocs);
	virtual ~Menu() {}

	// Draw the menu.
	virtual int Draw(GLShader shader) = 0;
};