#pragma once
#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu() {}
	~MainMenu() {}
	int Draw(GLShader shader);

private:
	int numberOfButtons = 3;

	unsigned int tex = Texture("../res/textures/debug.png").GetTextureId(); //Texture("../res/textures/MainMenu_Button1.png").GetTextureId(); 

	float buttonWidth = 0.6f;
	float buttonHeight = 0.3f;
	float buttonOffset = 0.5f;
	const float offsetChange = 0.5f;
};