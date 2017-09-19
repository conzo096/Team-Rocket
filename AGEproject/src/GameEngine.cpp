#include "GameEngine.h"
#include <assert.h>

GameEngine *GameEngine::instance = 0;

void GameEngine::Start()
{
	glfwInit();
	// Create a windowed mode window with hard coded parameters.
	window = glfwCreateWindow(1920, 1080, "Team Rocket", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		assert(!window && "window is not initalised");
		return;
	}

	// Window is now initalised, now make it the current context.
	glfwMakeContextCurrent(window);

	//// Loop through the game until the window is closed.
	while (!glfwWindowShouldClose(window))
	{
		// Clear the opengl buffer.
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap the window buffers.
		glfwSwapBuffers(window);

		// process events.
		glfwPollEvents();
	}
	//
	//// Application is over, handle resource cleaning.
	CleanUp();
}



void GameEngine::CleanUp()
{
	glfwTerminate();

}