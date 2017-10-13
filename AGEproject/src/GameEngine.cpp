#include "GameEngine.h"
#include <assert.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Shader.h"
#include "FileIO.h"
Shader *Shader::instance = nullptr;
GameEngine *GameEngine::instance = nullptr;



void GameEngine::Initialise()
{
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: glfw failed init! exiting.");
		return;
	}
	FileIO io = FileIO::get();
	io.LoadIniFile();
	// Create a windowed mode window with hard coded parameters.
	if(instance->fullScreen == false)
		instance->window = glfwCreateWindow(instance->GetScreenWidth(), instance->GetScreenHeight(), "Team Rocket", nullptr, nullptr);
	else
		instance->window = glfwCreateWindow(instance->GetScreenWidth(), instance->GetScreenHeight(), "Team Rocket", glfwGetPrimaryMonitor(), nullptr);
	
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
		fprintf(stderr, "ERROR: %p EXITING!", glewGetErrorString(glewInit()));
		return;
	}
	// glExperimental throws junk errors, Ignore.
	glGetError();
	PrintGlewInfo();
	LoadShaders();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// V-Sync, does not run without it
	glfwSwapInterval(1.0f);
}

void GameEngine::Render(glm::mat4 m, Model model, Effect effect)
{
	const auto mvp = instance->cameraMVP * m;

	// Switch back to basic when necessary
	Shader::Instance()->UseShader("Phong", effect, mvp, m, m, instance->cameraPos);
	if (&model != nullptr)
	{
		if (!model.GetStrip())
			model.Draw();
		else
			model.DrawStrip();
	}
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
	printf("-------------------------------------------------------\n");
//	printf("Glew version: %p\n", glewGetString(GLEW_VERSION));

	std::clog << "GL Version: " << glGetString(GL_VERSION) << std::endl;
	std::clog << "GL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::clog << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::clog << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	printf("-------------------------------------------------------\n");
}

void GameEngine::LoadShaders()
{
	//Shader::Instance()->AddShader("Basic");
	Shader::Instance()->AddShader("Phong");
}
