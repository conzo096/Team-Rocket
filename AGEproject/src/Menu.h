// Generic menu structure. 
#pragma once
#include <vector>
#include <string>
#include <glm\gtc\type_ptr.hpp>
#include "Texture.h"
#include "UserControls.h"
#include "Quad.h"
#include "GLShader.h"

struct Button
{
	// Texture of the button.
	Texture texture;
	// What action the button does.
	int action;
	// Quad that the button is rendered on.
	Quad renderTarget;
};

static class TMenu
{
public:
	TMenu() {}
	TMenu(std::vector<char *> textureLocs);

	static std::vector<Button> buttons;
	static int currentSelection;

	// Move up
	static void SelectionUp();
	// Move down.
	static void SelectionDown();
	// Return selected button action.
	static int SelectionPicked();

	// Draw the menu.
	static int Draw(GLShader shader);

};