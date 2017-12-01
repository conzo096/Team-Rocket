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
	static int currentSelection;

	int Draw(GLShader shader);

	void SelectionUp();
	void SelectionDown();
	void SelectionLeft();
	void SelectionRight();
	int SelectionPicked();

private:

	// Buttons which display their key binding.
	static std::vector <std::pair<Button, UIQuad>> buttons;
	// Texture for the buttons.
	std::vector <unsigned int> textureIds;

	// Button to leave controls menu.
	Button exit; 


	// Key bindings, index of buttons represents one of these.
	static std::vector<std::string> bindings;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void PopulateBindings();


};