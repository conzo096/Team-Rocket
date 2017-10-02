#include "GameEngine.h"
#include "Model.h"
#include <assert.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "UserControls.h"
#include "Shader.h"
#include "BoundingBox.h"
GameEngine *GameEngine::instance = 0;
Shader *Shader::instance = 0;

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
	LoadShaders();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);
}


void GameEngine::Render(glm::mat4 m, Model model, Effect effect)
{
	
	// Clear the opengl buffer.
	glClearColor(0.1, 0.0, 0.4, 1);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	UserControls controls;
	controls.ResetKeyBindings(UserControls::KEYBOARD);
	controls.UpdateMousePosition();
	controls.Update();
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080, 0.1f, 1000.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(90, 0, 200), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	if (glfwGetMouseButton(Instance()->GetWindow(), controls.GetKey("Action")))
	{
		auto bb = std::make_unique<BoundingBox>();
		bb->SetUpBoundingBox(model.GetVertexPositions());
		bb->SetColour(glm::vec4(1, 0, 0, 1));
		GLShader helloShader;
		if (!helloShader.AddShaderFromFile("../res/shaders/colour.vert", GLShader::VERTEX))
			std::printf("Vert failed to compile.\n");
		if (!helloShader.AddShaderFromFile("../res/shaders/colour.frag", GLShader::FRAGMENT))
			std::printf("Frag failed to compile.\n");
		helloShader.Link();
		helloShader.Use();
		glUniformMatrix4fv(helloShader.GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(Projection*View*m));
		glUniform4fv(helloShader.GetUniformLocation("colour"), 1, glm::value_ptr(bb->GetColour()));
		//std::printf("Checking for pixel");
		bb->Render();
		int i = controls.GetPickedColorIndexUnderMouse();
		std::printf("%i \n", i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		}
		auto mvp = Projection*View*m;
		Shader::Instance()->UseShader("Basic", effect, mvp);

		model.Draw();
		// process events.
		glfwPollEvents();
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
	Shader::Instance()->AddShader("Basic");
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
	unsigned char *data = stbi_load("../res/textures/debug.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); //Now remember to do something with this texture we got
	Shader::Instance()->AddShader("Basic");
	return texture;
}