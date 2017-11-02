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

	const int numOfTitles = 1;
	const int numOfOptions = 0;
	const int numOfSmallButtons = 0;
	const int numOfLargeButtons = 0;

	const float titleWidth = 1.0f;
	const float titleHeight = 0.6f;
	const float optionWidth = 0;
	const float optionHeight = 0;
	const float buttonWidth_Small = 0;
	const float buttonHeight_Small = 0;
	const float buttonWidth_Large = 0;
	const float buttonHeight_Large = 0;

	float titleOffsetX = 1.0f;
	float titleOffsetY = 0.8f;
	float optionOffsetX;
	float optionOffsetY;
	float buttonOffsetX_Small;
	float buttonOffsetY_Small;
	float buttonOffsetX_Large;
	float buttonOffsetY_Large;

	const float changeOffset_Title = 0;
	const float changeOffset_Option = 0;
	const float changeOffset_ButtonX_Small = 0;
	const float changeOffset_ButtonY_Small = 0;
	const float changeOffset_ButtonX_Large = 0;
	const float changeOffset_ButtonY_Large = 0;
};