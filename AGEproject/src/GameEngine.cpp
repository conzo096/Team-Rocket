#include "GameEngine.h"
#include <assert.h>

GameEngine *GameEngine::instance = 0;


void GameEngine::Initialise()
{
	if (!glfwInit())
	{
		std::fprintf(stderr, "ERROR: glfw failed init! exiting.");
		return;
	}

	// Create a windowed mode window with hard coded parameters.
	instance->window = glfwCreateWindow(1920, 1080, "Team Rocket", NULL, NULL);
	// Window is now initalised, now make it the current context.
	glfwMakeContextCurrent(instance->window);
	if (!instance->window)
	{
		assert(instance->window != NULL);
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
}

void GameEngine::Render()
{
	// Clear the opengl buffer.
	glClear(GL_COLOR_BUFFER_BIT);

	// Swap the window buffers.
	glfwSwapBuffers(instance->window);

	// process events.
	glfwPollEvents();
}

void GameEngine::Start()
{
	// Application is over, handle resource cleaning.
	//CleanUp();
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
