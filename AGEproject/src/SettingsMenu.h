#pragma once
#include "Menu.h"

class SettingsMenu : public Menu
{
public:
	SettingsMenu() {}
	~SettingsMenu() {}
	int Draw(GLShader shader);

private:
	unsigned int tex = Texture("../res/textures/debug.png").GetTextureId(); //Texture("../res/textures/MainMenu_Button1.png").GetTextureId(); 

};