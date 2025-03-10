#pragma once
#include "Menu.h"
#include "UIQuad.h"

class ControlsMenu : public Menu
{
public:
	ControlsMenu()
	{
		button_tex.push_back(ResourceHandler::Get().GetTexture("Forward"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Backward"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Left"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Right"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("RotateLeft"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("RotateRight"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Reset"));

		button_tex.push_back(ResourceHandler::Get().GetTexture("ZoomIn"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("ZoomOut"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Hold"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey1"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey2"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey3"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Select"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Back"));

		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Forward_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Backward_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Left_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Right_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("RotateLeft_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("RotateRight_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Reset_HIGHLIGHTED"));

		highlight_tex.push_back(ResourceHandler::Get().GetTexture("ZoomIn_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("ZoomOut_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Hold_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("HotKey1_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("HotKey2_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("HotKey3_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Select_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Back_HIGHLIGHTED"));

		current_tex = button_tex;

		if (buttons.size() == 0)
		{
			DrawButtons();
		}
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
	static int lastSelection;
	static const int numOfControls;

	bool isControllerCallBack = false;

	// Buttons which display their key binding.
	static std::vector<std::pair<Button, UIQuad>> buttons;
	// Button textures
	static std::vector <unsigned int> button_tex;
	static std::vector <unsigned int> highlight_tex;
	static std::vector <unsigned int> current_tex;

	// Key bindings, index of buttons represents one of these.
	static std::vector<std::pair<std::string, std::string>> bindings;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void ControllerCallBack();
	void PopulateBindings();
	void DrawButtons();
};