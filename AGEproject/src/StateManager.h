#pragma once
#define GLEW_STATIC
#include "GameEngine.h"
#include "Singleton.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MainMenu.h"

class StateManager : public Singleton<StateManager>
{
	enum State
	{
		stateSplash,
		stateMainMenu,
		stateSettings, //May be a part of menu
		statePlaying,
		stateExiting
	};

public:

	State state;

	void StateLoop();

	int ShowMainMenu()
	{
		MainMenu mm;
		return mm.Draw(Shader::Get().GetShader("tex"));
	}

	int ShowSettings()
	{

	}

private:
	int select;
	bool running;
};