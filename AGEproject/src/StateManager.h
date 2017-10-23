#pragma once
#define GLEW_STATIC
#include "GameEngine.h"
#include "Singleton.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Menu.h"

class StateManager : public Singleton<StateManager>
{
	enum State
	{
		Splash,
		MainMenu,
		Settings, //May be a part of menu
		Playing,
		Exiting
	};

public:
	State state;

	void StateLoop();

	void ShowMainMenu()
	{
		Menu t;
		t.Draw(Shader::Get().GetShader("tex"));
	}

private:


};