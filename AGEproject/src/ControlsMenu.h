#pragma once
#include "Menu.h"
#include "UIQuad.h"

class ControlsMenu : public Menu
{
public:
	ControlsMenu() 
	{
		//button_tex.push_back(ResourceHandler::Get().GetTexture("Forward"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("Backward"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("Left"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("Right"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("RotateLeft"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("RotateRight"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("ZoomIn"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("ZoomOut"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("Hold"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey1"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey2"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey3"));

		//button_tex.push_back(ResourceHandler::Get().GetTexture("Forward_HIGHLIGHTED"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("Backward_HIGHLIGHTED"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("Left_HIGHLIGHTED"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("Right_HIGHLIGHTED"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("RotateLeft_HIGHLIGHTED"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("RotateRight_HIGHLIGHTED"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("ZoomIn_HIGHLIGHTED"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("ZoomOut_HIGHLIGHTED"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("Hold_HIGHLIGHTED"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey1_HIGHLIGHTED"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey2_HIGHLIGHTED"));
		//button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey3_HIGHLIGHTED"));
	}
	~ControlsMenu() {}

	int Draw(GLShader shader);

	void SelectionUp();
	void SelectionDown();
	void SelectionLeft();
	void SelectionRight();
	int SelectionPicked();

private:
	static int currentSelection;

	// Buttons which display their key binding.
	static std::vector<std::pair<Button, UIQuad>> buttons;
	std::vector <unsigned int> button_tex;

	// Button to leave controls menu.
	Button exit; 

	// Key bindings, index of buttons represents one of these.
	static std::vector<std::string> bindings;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void PopulateBindings();
};