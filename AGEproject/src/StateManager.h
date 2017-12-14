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

	int ShowSplashScreen()
	{
		// Initialise required assets, time, shader, quad, texture
	//	const std::chrono::milliseconds timeToWait(5000); // 5 Seconds
		clock_t t = clock();
		float elapsedTime = 0;
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


		while (true)
		{
			t = clock() - t;
			elapsedTime = (float)t / CLOCKS_PER_SEC;
			if (elapsedTime > 5.0f)
				// Show the main menu
				return stateMainMenu;
		}
		//// After drawing, start waiting
		//std::this_thread::sleep_for(timeToWait);
		return stateMainMenu;
	}

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

	int ShowPauseScreen()
	{
		SettingsMenu sm;
		return sm.Draw(*ResourceHandler::Get().GetShader("Basic"));
	}

};