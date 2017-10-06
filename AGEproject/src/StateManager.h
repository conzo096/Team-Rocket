#pragma once
#define GLEW_STATIC
#include "GameEngine.h"
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Menu.h"
#include "Game.h"
#include "Entity.h"
#include "Renderable.h"
class StateManager
{
	enum State
	{
		Splash,
		Menu,
		Settings, //May be a part of menu
		Playing,
		Exiting
	};

	static StateManager *instance;

public:
	static StateManager *Instance()
	{
		if (!instance)
		{
			instance = new StateManager();
		}
		return instance;
	}

	State state;

	void StateLoop();


private:

	void ShowMainMenu()
	{
		TMenu t;
		t.Draw(Shader::GetShader(std::string("Basic")));
	}

};