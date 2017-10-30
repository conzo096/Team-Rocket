// Generic menu structure. 
#pragma once
#include <vector>
#include <string>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc/matrix_transform.hpp>
#include "UserControls.h"
#include "GLShader.h"
#include "Texture.h"
#include "Quad.h"
#include "Menu_Camera.h"

struct Button
{
	// Texture of the button.
	unsigned int texture;
	// What action the button does.
	int action;
	// Quad that the button is rendered on.
	Quad renderTarget;
};

class Menu
{
protected:
	Entity* menu_cam;
	bool selectionMade;

public:
	Menu()
	{
		selectionMade = false;
		menu_cam = new Entity;

		auto cam = std::make_unique<Menu_Camera>();
		cam->SetPosition(glm::dvec3(0.0, 0.0, 100.0));
		cam->SetTarget(glm::vec3(0, 0, 0));
		cam->SetProjection(glm::half_pi<float>(), (GameEngine::Get().GetScreenWidth() / GameEngine::Get().GetScreenHeight()), 2.414f, 1000);

		menu_cam->AddComponent(move(cam));
	}
	//Menu(std::vector<char *> textureLocs);
	virtual ~Menu() {}

	std::vector<Button> buttons;
	int currentSelection = 0;

	// Move up
	void SelectionUp()
	{
		if (buttons.size() == 0)
			return;
		// If currentSelection is the first one, loop to end one. 
		if (currentSelection == 0)
			currentSelection = buttons.size() - 1;
		currentSelection -= 1;
	}

	// Move down.
	void SelectionDown()
	{
		if (buttons.size() == 0)
			return;
		// If currentSelection is the last one one, loop to first one. 
		if (currentSelection == buttons.size() - 1)
			currentSelection = 0;
		currentSelection += 1;
	}

	// Return selected button action.
	int SelectionPicked()
	{
		return buttons.at(currentSelection).action;
	}

	// Draw the menu.
	virtual int Draw(GLShader shader) = 0;
};