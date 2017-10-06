// Generic menu structure. 
#pragma once
#include <vector>
#include <string>
#include <glm\gtc\type_ptr.hpp>
#include "Texture.h"
#include "UserControls.h"
#include <glm\gtc/matrix_transform.hpp>
#include "Quad.h"
#include "GLShader.h"

struct Button
{
	// Texture of the button.
	unsigned int texture;
	// What action the button does.
	int action;
	// Quad that the button is rendered on.
	Quad renderTarget;
};

class TMenu
{
public:
	TMenu() {}
	TMenu(std::vector<char *> textureLocs);

	std::vector<Button> buttons;
	int currentSelection =0;

	// Move up
	void SelectionUp();
	// Move down.
	
	void SelectionDown();
	// Return selected button action.
	int SelectionPicked();

	// Draw the menu.
	int Draw(GLShader shader);

};