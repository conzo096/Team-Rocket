#pragma once
#define GLEW_STATIC
#include "GameEngine.h"
#include "Singleton.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MainMenu.h"
#include "SettingsMenu.h"

// Remove soon!!
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

class StateManager : public Singleton<StateManager>
{
	enum State
	{
		stateSplash,
		stateMainMenu,
		stateSettings,
		statePlaying,
		stateExiting
	};

public:

	State state;

	void StateLoop();

	int ShowMainMenu()
	{
		MainMenu mm;
		return mm.Draw(*ResourceHandler::Get().GetShader("Basic"));
	}

	int ShowSettingsMenu()
	{
		SettingsMenu sm;
		int val = sm.Draw(*ResourceHandler::Get().GetShader("Basic"));
		std::this_thread::sleep_for(0.15s);
		return val;
	}

	int ShowSplashScreen()
	{
		
		/*while(!UserControls::Get().IsMouseButtonPressed(std::string("Action")))
		{
			
		}*/
		return stateMainMenu;
	}
};