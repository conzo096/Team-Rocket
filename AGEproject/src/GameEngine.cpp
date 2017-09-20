#include "GameEngine.h"
#include <assert.h>
#include "GLShader.h"
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

	std::printf("-------------------------------\n");
	std::printf("Testing shaders\n");
	GLShader helloShader;
	if (!helloShader.AddShaderFromFile("../res/shaders/HelloWorld.vert", GLShader::VERTEX))
		std::printf("Vert failed to compile.\n");
	if(!helloShader.AddShaderFromFile("../res/shaders/HelloWorld.frag", GLShader::FRAGMENT))
		std::printf("Frag failed to compile.\n");
	helloShader.Link();
	
	
	float vertices[] = {
		0.0f,  0.5f, // Vertex 1 (X, Y)
		0.5f, -0.5f, // Vertex 2 (X, Y)
		-0.5f, -0.5f  // Vertex 3 (X, Y)
	};
	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glUseProgram(helloShader.GetId());
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindFragDataLocation(helloShader.GetId(), 0, "outColor");
	GLint posAttrib = glGetAttribLocation(helloShader.GetId(), "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);
	glDrawArrays(GL_TRIANGLES, 0, 3);


	std::printf("-------------------------------\n");

	// Loop through the game until the window is closed.
	while (!glfwWindowShouldClose(window))
	{
		// Clear the opengl buffer.
		//glClear(GL_COLOR_BUFFER_BIT);

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
