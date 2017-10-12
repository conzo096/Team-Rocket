#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLShader.h"
#include "stb_image.h"
#include <fstream>
#include <sstream>
#include "Material.h"
#include "Model.h"

struct Effect
{
	unsigned int texture;
	std::string shader;
	Material material;
};

class GameEngine
{
private:

	// Singleton instance of the Game Engine.
	static GameEngine *instance;
	// The window that is to be rendered too.
	GLFWwindow* window;
	float width;
	float height;
	bool fullScreen;
	glm::mat4 cameraMVP;

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
	static void Render(glm::mat4 mvp, Model model, Effect effect);


	// Getters for width and height
	float GetScreenWidth() { return width; }
	float GetScreenHeight() { return height; }
	bool GetFullScreen() { return fullScreen; }
	void SetFullScreen(int val) { instance->fullScreen = val; }
	void SetScreenWidth(int val) { instance->width = val; }
	void SetScreenHeight(int val) { instance->height = val; }

	void SetCamera(glm::mat4 camera);
	// Execute the game engine.
	void Start();
	// Cleans up game engine resources.
	static void CleanUp();


	// Helper functions.
	static void PrintGlewInfo();
	static void LoadShaders();

};