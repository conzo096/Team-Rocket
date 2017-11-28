#pragma once
#include "Menu.h"
#include "UIQuad.h"
class ControlsMenu : public Menu
{
public:
	ControlsMenu()
	{
	
	}
	~ControlsMenu() {}
	int currentSelection = 0;

	int Draw(GLShader shader);

	void SelectionUp();
	void SelectionDown();
	void SelectionLeft();
	void SelectionRight();
	int SelectionPicked();

private:

	// Buttons which display their key binding.
	std::vector <std::pair<Button, UIQuad>> buttons;
	// Texture for the buttons.
	std::vector <unsigned int> textureIds;

};