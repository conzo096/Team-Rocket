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
#include "UserControls.h"
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
		stateTutorial,
		statePlaying,
		stateExiting,
		statePause
	};

	State currentState;

	void StateLoop();

	void ShowSplashScreen()
	{
		// Initialise required assets, time, shader, quad, texture
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
		const clock_t start_time = clock();
		float elapsedTime = 0;
		while (elapsedTime < 2.0f)
		{
			elapsedTime = static_cast<float>(clock() - start_time) / CLOCKS_PER_SEC;
		}
		return stateMainMenu;
	}

	int ShowMainMenu()
	{
		MainMenu mm;
		return mm.Draw(*shader);
	}

	int ShowSettingsMenu()
	{
		SettingsMenu sm;
		return sm.Draw(*shader);
	}

	int ShowControlsMenu()
	{
		ControlsMenu cm;
		return cm.Draw(*shader);
	}

	void ShowTutorial()
	{
		GLShader shader = *ResourceHandler::Get().GetShader("Basic");
		bool selectionMade = false;
		bool mouseButtonHeld = false;

		Button newButton;
		newButton.texture = ResourceHandler::Get().GetTexture("Start_Game");
		newButton.renderTarget = Quad(glm::vec2(0.05f, -1 + 0.02f),
									  glm::vec2(0.75f, -1 + 0.4f + 0.02f));
		
		newButton.renderTarget.SetOpenGL();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);
		shader.Use();

		glUniform1i(shader.GetUniformLocation("tex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, newButton.texture);
		newButton.renderTarget.Draw();

		Quad quad = Quad();
		const unsigned int tex = ResourceHandler::Get().GetTexture("Tutorial");

		quad.SetOpenGL();
		glBindTexture(GL_TEXTURE_2D, tex);
		quad.Draw();

		glfwSwapBuffers(GameEngine::Get().GetWindow());

		int count = 0;
		while (!selectionMade)
		{
			count++;
			selectionMade = UserControls::Get().MouseSelection(std::string("Action"), newButton, mouseButtonHeld);
			if (count > 2000)
			{
				if (selectionMade)
					return;
				selectionMade = UserControls::Get().IsJoystickPressed("A");
			}
			if (selectionMade)
				return;
			glfwPollEvents();
		}
	}

private:
	GLShader* shader;
};