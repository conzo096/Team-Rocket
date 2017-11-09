#pragma once
#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu() 
	{
		numberOfButtons = 3;

		buttonWidth = 0.6f;
		buttonHeight = 0.3f;
		buttonOffset = 0.4f;
		offsetChange = buttonHeight + 0.2f;
	}
	~MainMenu() {}

	std::vector<Button> buttons;
	int currentSelection = 0;

	int Draw(GLShader shader);

	void SelectionUp();
	void SelectionDown();
	int SelectionPicked();

private:
	int numberOfButtons;

	float buttonWidth;
	float buttonHeight;
	float buttonOffset;
	float offsetChange;
};