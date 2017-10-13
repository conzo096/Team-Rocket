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
#include "Menu_Camera.h"
#include "Free_Camera.h"

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
private:
	// Camera that views menu (hopefully this will be shared in future)
	Entity* menu_cam;

public:
	TMenu() 
	{
		menu_cam = new Entity;

		auto cam = std::make_unique<Menu_Camera>();
		cam->SetPosition(glm::dvec3(0.0, 0.0, 100.0));
		cam->SetTarget(glm::vec3(0, 0, 0));
		cam->SetProjection(glm::half_pi<float>(), (GameEngine::Get().GetScreenWidth() / GameEngine::Get().GetScreenHeight()), 2.414f, 1000);

		menu_cam->AddComponent(move(cam));
	}
	TMenu(std::vector<char *> textureLocs);

	std::vector<Button> buttons;
	int currentSelection = 0;

	// Move up
	void SelectionUp();
	// Move down.
	void SelectionDown();
	// Return selected button action.
	int SelectionPicked();

	// Draw the menu.
	int Draw(GLShader shader);
};