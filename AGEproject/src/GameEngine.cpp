#include "GameEngine.h"
#include "Free_Camera.h"
#include <assert.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
GameEngine *GameEngine::instance = 0;
Free_Camera cam(glm::radians(45.0f));

void GameEngine::Initialise()
{
	if (!glfwInit())
	{
		std::fprintf(stderr, "ERROR: glfw failed init! exiting.");
		return;
	}

	// Create a window
	instance->window = glfwCreateWindow(GameEngine::Instance()->GetScreenWidth(), 
										GameEngine::Instance()->GetScreenHeight(), 
										"Team Rocket", NULL, NULL);
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
	LoadShaders();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void GameEngine::Render(glm::mat4 m, Model model, unsigned int texture)
{
	// Clear the opengl buffer.
	glClearColor(0.1, 0.0, 0.4, 1);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_CULL_FACE);
	GLShader helloShader;
	if (!helloShader.AddShaderFromFile("../res/shaders/BasicVert.vert", GLShader::VERTEX))
		std::printf("Vert failed to compile.\n");
	if (!helloShader.AddShaderFromFile("../res/shaders/BasicFrag.frag", GLShader::FRAGMENT))
		std::printf("Frag failed to compile.\n");

	glUniform1i(helloShader.GetUniformLocation("tex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	helloShader.Link();
	helloShader.Use();

	auto aspect = static_cast<float>(GameEngine::Instance()->GetScreenWidth()) / static_cast<float>(GameEngine::Instance()->GetScreenHeight());
	cam.SetPosition(glm::vec3(120, 3, 30));
	cam.SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	cam.SetOrientation(glm::vec3(0, 1, 0));
	cam.SetProjection(aspect, 0.1f, 1000.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	auto mvp = cam.GetProjection() * cam.GetView() * m;

	//LoadTextures(helloShader);

	glUniformMatrix4fv(helloShader.GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	model.Draw(helloShader);
	// process events.
	glfwPollEvents();
	// Swap the window buffers.
	glfwSwapBuffers(instance->window);

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

void GameEngine::LoadShaders()
{
	std::printf("-------------------------------\n");
	std::printf("Testing shaders\n");
	GLShader helloShader;
	if (!helloShader.AddShaderFromFile("../res/shaders/HelloWorld.vert", GLShader::VERTEX))
		std::printf("Vert failed to compile.\n");
	if (!helloShader.AddShaderFromFile("../res/shaders/HelloWorld.frag", GLShader::FRAGMENT))
		std::printf("Frag failed to compile.\n");
	helloShader.Link();
	std::printf("-------------------------------\n");
}

unsigned int GameEngine::LoadTextures(const char* location)
{
	unsigned int texture;
	// texture 1
	// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(location, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}
