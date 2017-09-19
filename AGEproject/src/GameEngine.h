#pragma once
#include <GLFW/glfw3.h>
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


	// Execute the game engine.
	void Start();
	// Cleans up game engine resources.
	void CleanUp();

};