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

<<<<<<< HEAD
void GameEngine::Render()
{
	// Clear the opengl buffer.
	glClear(GL_COLOR_BUFFER_BIT);

	// Swap the window buffers.
	glfwSwapBuffers(instance->window);
=======
	std::printf("-------------------------------\n");
	std::printf("Testing shaders\n");
	GLShader helloShader;
	if (!helloShader.AddShaderFromFile("../res/shaders/HelloWorld.vert", GLShader::VERTEX))
		std::printf("Vert failed to compile.\n");
	if(!helloShader.AddShaderFromFile("../res/shaders/HelloWorld.frag", GLShader::FRAGMENT))
		std::printf("Frag failed to compile.\n");
	helloShader.Link();
	std::printf("-------------------------------\n");

	// Loop through the game until the window is closed.
	while (!glfwWindowShouldClose(window))
	{
		// Clear the opengl buffer.
		glClear(GL_COLOR_BUFFER_BIT);
>>>>>>> parent of ae157ae... Shaders work!

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
