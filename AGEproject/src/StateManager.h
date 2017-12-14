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
#include <thread>
#include <chrono>


class StateManager : public Singleton<StateManager>
{
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

public:

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
		// Initialise required assets, time, shader, quad, texture
		const std::chrono::milliseconds timeToWait(5000); // 5 Seconds
		GLShader shader = *ResourceHandler::Get().GetShader("Basic");
		Quad quad = Quad();
		const unsigned int tex = ResourceHandler::Get().GetTexture("Splash_Screen");

		// Do OpenGL & shader things
		quad.SetOpenGL();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);
		shader.Use();

		// Do texture & quad things
		glBindTexture(GL_TEXTURE_2D, tex);
		quad.Draw();

		// Render it
		glfwSwapBuffers(GameEngine::Get().GetWindow());
		glfwPollEvents();

		// After drawing, start waiting
		std::this_thread::sleep_for(timeToWait);

		// Show the main menu
		return stateMainMenu;
	}

	int ShowPauseScreen()
	{
		SettingsMenu sm;
		sm.Draw(*ResourceHandler::Get().GetShader("Basic"));

		return statePlaying;
	}

};