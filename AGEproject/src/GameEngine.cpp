#include "GameEngine.h"
#include <assert.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Shader.h"
#include "FileIO.h"

void GameEngine::Initialise()
{
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: glfw failed init! exiting.");
		return;
	}
	FileIO io = FileIO::Get();
	io.LoadIniFile();
	// Create a windowed mode window with hard coded parameters.
	if(fullScreen == false)
		window = glfwCreateWindow(GetScreenWidth(), GetScreenHeight(), "Team Rocket", NULL, NULL);
	else
		window = glfwCreateWindow(GetScreenWidth(),GetScreenHeight(), "Team Rocket", glfwGetPrimaryMonitor(), NULL);
	
	// Window is now initalised, now make it the current context.
	glfwMakeContextCurrent(Get().window);
	if (!Get().window)
	{
		assert(Get().window != NULL);
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
	const auto mvp = Get().cameraMVP * m;
//	Shader::Get().UseShader("Basic", effect, mvp);
	Shader::Get().UseShader("Phong", effect, mvp, m, m, cameraPos);
	model.Draw();
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
	Shader::Get().AddShader("Phong");
}
