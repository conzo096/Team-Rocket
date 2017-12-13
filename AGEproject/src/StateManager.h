#pragma once
#define GLEW_STATIC
#include "GameEngine.h"
#include "Singleton.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "ControlsMenu.h"
class StateManager : public Singleton<StateManager>
{
public:
	enum State
	{
		stateSplash,
		stateMainMenu,
		stateSettings,
		stateControls,
		statePlaying,
		stateExiting,
		statePause
	};

	State currentState;

	void StateLoop();

	int ShowMainMenu()
	{
		MainMenu mm;
		return mm.Draw(*ResourceHandler::Get().GetShader("Basic"));
	}

	int ShowSettingsMenu()
	{
		SettingsMenu sm;
		return sm.Draw(*ResourceHandler::Get().GetShader("Basic"));
	}

	int ShowControlsMenu()
	{
		ControlsMenu sm;
		return sm.Draw(*ResourceHandler::Get().GetShader("Basic"));
	}

	int ShowSplashScreen()
	{
		return stateMainMenu;
	}

	int ShowPauseScreen()
	{
		SettingsMenu sm;
		return sm.Draw(*ResourceHandler::Get().GetShader("Basic"));
	}

};