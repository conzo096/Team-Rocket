#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLShader.h"
#include "stb_image.h"
#include <fstream>
#include <sstream>
#include "Model.h"

class GameEngine
{
private:

	// Singleton instance of the Game Engine.
	static GameEngine *instance;
	// The window that is to be rendered too.
	GLFWwindow* window;



public:
	// Constructor, if singleton has not been initalised, initalise it, else return instance.
	static GameEngine *Instance()
	{
		if (!instance)
			instance = new GameEngine();
		return instance;
	}


	// The render window.
	GLFWwindow* GetWindow() { return instance->window; }

	
	static void Initialise();
	static void Render(Model model, unsigned int texture);

	// Execute the game engine.
	void Start();
	// Cleans up game engine resources.
	static void CleanUp();




	// Helper functions.
	static void PrintGlewInfo();
	static void LoadShaders();

	static unsigned int LoadTextures(const char* location);
};