#pragma once
#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu() {}
	~MainMenu() {}

	std::vector<Button> buttons;
	int currentSelection = 0;

	int Draw(GLShader shader);

	void SelectionUp();
	void SelectionDown();
	int SelectionPicked();

private:
	const int numberOfButtons = 3;

	unsigned int tex = Texture("../res/textures/debug.png").GetTextureId(); //Texture("../res/textures/MainMenu_Button1.png").GetTextureId(); 

	const float buttonWidth = 0.6f;
	const float buttonHeight = 0.3f;
	float buttonOffset = 0.5f;
	const float offsetChange = 0.5f;
};