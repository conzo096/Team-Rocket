#include "GameEngine.h"
#include <assert.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Shader.h"
#include "FileIO.h"
GameEngine *GameEngine::instance = 0;
Shader *Shader::instance = 0;

void GameEngine::Initialise()
{
	if (!glfwInit())
	{
		std::fprintf(stderr, "ERROR: glfw failed init! exiting.");
		return;
	}
	FileIO io = FileIO::get();
	io.LoadIniFile();
	// Create a windowed mode window with hard coded parameters.
	if(instance->fullScreen == false)
		instance->window = glfwCreateWindow(instance->GetScreenWidth(), instance->GetScreenHeight(), "Team Rocket", NULL, NULL);
	else
		instance->window = glfwCreateWindow(instance->GetScreenWidth(), instance->GetScreenHeight(), "Team Rocket", glfwGetPrimaryMonitor(), NULL);
	
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
	glfwSwapInterval(1.0f);
}

void GameEngine::Render(glm::mat4 m, Model model, Effect effect)
{
	auto mvp = instance->cameraMVP * m;
	Shader::Instance()->UseShader("Basic", effect, mvp);
	if (!model.GetStrip())
		model.Draw();
	else
		model.DrawStrip();
}

void GameEngine::SetCamera(glm::mat4 camera)
{
	cameraMVP = camera;
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
	Shader::Instance()->AddShader("Basic");
}
