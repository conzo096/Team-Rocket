#include "GameEngine.h"
#include <assert.h>

GameEngine *GameEngine::instance = 0;

void GameEngine::Start()
{
	
	if (!glfwInit())
	{
		std::fprintf(stderr, "ERROR: glfw failed init! exiting.");
		return;
	}
	// Create a windowed mode window with hard coded parameters.
	window = glfwCreateWindow(1920, 1080, "Team Rocket", NULL, NULL);
	// Window is now initalised, now make it the current context.
	glfwMakeContextCurrent(window);
	if (!window)
	{
		assert(window != NULL);
		CleanUp();
		return;
	}

	// Set up glew.
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::fprintf(stderr, "ERROR: %s EXITING!", glewGetErrorString(glewInit()));
		return;
	}
	// glExperimental throws junk errors, Ignore.
	glGetError();
	PrintGlewInfo();

	// Loop through the game until the window is closed.
	while (!glfwWindowShouldClose(window))
	{
		// Clear the opengl buffer.
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap the window buffers.
		glfwSwapBuffers(window);

		// process events.
		glfwPollEvents();
	}
	
	// Application is over, handle resource cleaning.
	CleanUp();
}



void GameEngine::CleanUp()
{
	glfwTerminate();

}


void GameEngine::PrintGlewInfo()
{
	std::printf("-------------------------------------------------------\n");
	std::printf("Glew version: %s\n", glewGetString(GLEW_VERSION));
	std::printf("Gl version: %s\n", glGetString(GL_VERSION));
	std::printf("Vendor: %s\n", glGetString(GL_VENDOR));
	std::printf("Graphics card: %s\n", glGetString(GL_RENDERER));
	std::printf("Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::printf("-------------------------------------------------------\n");
}
