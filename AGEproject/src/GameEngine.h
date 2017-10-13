#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include <sstream>
#include "Model.h"

class Material;

struct Effect
{
	unsigned int texture;
	std::string shader;
	Material* material;
};

class GameEngine
{
	// Singleton instance of the Game Engine.
	static GameEngine *instance;
	// The window that is to be rendered too.
	GLFWwindow* window;
	unsigned int width;
	unsigned int height;
	bool fullScreen;
	glm::mat4 cameraMVP;
	glm::vec3 cameraPos;

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
	unsigned int GetScreenWidth() { return width; }
	unsigned int GetScreenHeight() { return height; }
	bool GetFullScreen() { return fullScreen; }
	void SetFullScreen(int val) { instance->fullScreen = val; }
	void SetScreenWidth(int val) { instance->width = val; }
	void SetScreenHeight(int val) { instance->height = val; }
	void SetCameraPos(glm::vec3 pos) { instance->cameraPos = pos; }

	void SetCamera(glm::mat4 camera);
	// Execute the game engine.
	void Start();
	// Cleans up game engine resources.
	static void CleanUp();


	// Helper functions.
	static void PrintGlewInfo();
	static void LoadShaders();

};