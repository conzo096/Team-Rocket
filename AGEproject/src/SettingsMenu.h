#pragma once
#include "Menu.h"

class SettingsMenu : public Menu
{
public:
	SettingsMenu() {}
	~SettingsMenu() {}

	std::vector<Label> labels;
	std::vector<Button> buttons;
	int currentSelection = 0;

	int Draw(GLShader shader);

	void SelectionUp();
	void SelectionDown();
	void SelectionLeft();
	void SelectionRight();
	int SelectionPicked();

private:
	unsigned int tex = Texture("../res/textures/debug.png").GetTextureId(); //Texture("../res/textures/MainMenu_Button1.png").GetTextureId(); 

	const int numOfTitles;
	const int numOfOptions;
	const int numOfSmallButtons;
	const int numOfLargeButtons;

	const float titleWidth;
	const float titleHeight;
	float titleOffsetX;
	float titleOffsetY;
	const float changeOffset_Title;

	const float optionWidth;
	const float optionHeight;
	float optionOffsetX;
	float optionOffsetY;
	const float changeOffset_Option;
	
	const float buttonWidth_Small;
	const float buttonHeight_Small;
	float buttonOffsetX_Small;
	float buttonOffsetY_Small;
	const float changeOffset_ButtonX_Small;
	const float changeOffset_ButtonY_Small;

	const float buttonWidth_Large;
	const float buttonHeight_Large;
	float buttonOffsetX_Large;
	float buttonOffsetY_Large;
	const float changeOffset_ButtonX_Large;
	const float changeOffset_ButtonY_Large;
};