#pragma once
#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu() 
	{
		currentSelection = -1;
		numberOfButtons = 3;

		normal_tex[0] = ResourceHandler::Get().GetTexture("Start_Game");
		normal_tex[1] = ResourceHandler::Get().GetTexture("Options");
		normal_tex[2] = ResourceHandler::Get().GetTexture("Exit_Game");

		highlight_tex[0] = ResourceHandler::Get().GetTexture("Start_Game_HIGHLIGHTED");
		highlight_tex[1] = ResourceHandler::Get().GetTexture("Options_HIGHLIGHTED");
		highlight_tex[2] = ResourceHandler::Get().GetTexture("Exit_Game_HIGHLIGHTED");

		buttonWidth = 0.6f;
		buttonHeight = 0.3f;
		buttonOffset = 0.4f;
		offsetChange = buttonHeight + 0.2f;
	}
	~MainMenu() {}

	std::vector<Button> buttons;

	int Draw(GLShader shader);

	void SelectionUp();
	void SelectionDown();
	int SelectionPicked();

private:
	int currentSelection;
	
	int numberOfButtons;

	unsigned int normal_tex[3];
	unsigned int highlight_tex[3];

	float buttonWidth;
	float buttonHeight;
	float buttonOffset;
	float offsetChange;
};