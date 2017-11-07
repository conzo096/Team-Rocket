#pragma once
#include "Menu.h"

class SettingsMenu : public Menu
{
public:
	SettingsMenu()
	{
		title_tex[0] = Texture("../res/textures/SettingsMenu_Title1.png").GetTextureId();
		title_tex[1] = Texture("../res/textures/SettingsMenu_Title2.png").GetTextureId();
		option_tex[0][0] = Texture("../res/textures/SettingsMenu_Option1-1.png").GetTextureId();
		option_tex[1][0] = Texture("../res/textures/SettingsMenu_Option2-1.png").GetTextureId();
		small_button_tex[0] = Texture("../res/textures/SettingsMenu_SmallButton1.png").GetTextureId();
		small_button_tex[1] = Texture("../res/textures/SettingsMenu_SmallButton2.png").GetTextureId();
		large_button_tex[0] = Texture("../res/textures/SettingsMenu_LargeButton1.png").GetTextureId();
		large_button_tex[1] = Texture("../res/textures/SettingsMenu_LargeButton2.png").GetTextureId();
		large_button_tex[2] = Texture("../res/textures/SettingsMenu_LargeButton3.png").GetTextureId();
		
		numOfTitles = 2;
		numOfOptions = 2;
		numOfLargeButtons = 3;
		numOfSmallButtons = 4;

		titleWidth = 0.5f;
		titleHeight = 0.18f;
		optionWidth = 0.3f;
		optionHeight = 0.15f;
		buttonHeight_Large = titleHeight;
		buttonWidth_Small = 0.03f;
		buttonHeight_Small = 0.05f;

		titleOffsetX = 0.5f;
		titleOffsetY = 0.33f;
		optionOffsetX = 0.55f;
		optionOffsetY = titleOffsetY + 0.015f;
		buttonOffsetX_Large = 0.25f;
		buttonOffsetY_Large = 0.0f;
		buttonOffsetX_Small = 0.5f;
		buttonOffsetY_Small = optionOffsetY + (optionHeight / 2.0f) - (buttonHeight_Small / 2.0f);

		changeOffset_Title = 0.11f;
		changeOffset_Option = 0.14f;
		changeOffset_Button_Large = changeOffset_Title;
		changeOffset_ButtonX_Small = 0.0395f + optionWidth;
		changeOffset_ButtonY_Small = (titleHeight / 2.0f) + changeOffset_Title + (optionHeight / 2.0f) - (buttonHeight_Small / 2.0f) - 0.01f;
	}
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

	unsigned int title_tex[2];
	unsigned int option_tex[2][1];
	unsigned int small_button_tex[2];
	unsigned int large_button_tex[3];
	
	int numOfTitles;
	int numOfOptions;
	int numOfSmallButtons;
	int numOfLargeButtons;

	float titleWidth;
	float titleHeight;
	float optionWidth;
	float optionHeight;
	float buttonWidth_Small;
	float buttonHeight_Small;
	float buttonWidth_Large;
	float buttonHeight_Large;

	float titleOffsetX;
	float titleOffsetY;
	float optionOffsetX;
	float optionOffsetY;
	float buttonOffsetX_Small;
	float buttonOffsetY_Small;
	float buttonOffsetX_Large;
	float buttonOffsetY_Large;

	float changeOffset_Title;
	float changeOffset_Option;
	float changeOffset_ButtonX_Small;
	float changeOffset_ButtonY_Small;
	float changeOffset_Button_Large;
};