#pragma once
#include "GameEngine.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

};