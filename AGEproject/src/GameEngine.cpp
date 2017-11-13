#include "GameEngine.h"
#include <assert.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
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
	//LoadShaders();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// V-Sync, does not run without it
	glfwSwapInterval(1.0);
}

//void GameEngine::Render(glm::mat4 m, Model model, Effect effect)
//{
//	const auto mvp = cameraMVP * m;
////	Shader::Get().UseShader("Basic", effect, mvp);
////	Shader::Get().UseShader("Phong", effect, mvp, m, m, cameraPos);
//	model.Draw();
//}

void GameEngine::Render()
{
	glClearColor(0.1f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	for (RenderData rl : renderList)
	{
		// Bind shader.
		glUseProgram(rl.shader);
		// Bind Uniforms.
		const auto mvp = cameraMVP * rl.m;

		GLint index;
		index = glGetUniformLocation(rl.shader, "MVP");
		glUniformMatrix4fv(index, 1, GL_FALSE, value_ptr(mvp));
		index = glGetUniformLocation(rl.shader, "M");
		glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(rl.m));
		index = glGetUniformLocation(rl.shader, "N");
		glUniformMatrix3fv(index, 1, GL_FALSE, glm::value_ptr(glm::mat3(rl.m)));
		index = glGetUniformLocation(rl.shader, "eye_pos");
		glm::vec3 eyePos = glm::vec3(mvp[0][3],mvp[1][3],mvp[2][3]);
		glUniform3fv(index, 1, glm::value_ptr(eyePos));

		// Bind material.
		BindMaterial(rl.mat, rl.shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rl.texture);
		

		// Bind and draw model.
		glBindVertexArray(rl.modelVao);
		glDrawElements(rl.drawType, rl.indices, GL_UNSIGNED_INT, 0);
	}


	unsigned int shaderId = ResourceHandler::Get().GetShader("Particle")->GetId();
	// Now render particles.
	glUseProgram(shaderId);
	// Now render all particles.
	for (ParticleData &p : particles)
	{
		// Bind uniforms.
		GLint index;
		index = glGetUniformLocation(shaderId, "VP");
		glUniformMatrix4fv(index, 1, GL_FALSE, value_ptr(cameraMVP));
		index = glGetUniformLocation(shaderId, "up");
		glUniform3fv(index, 1, glm::value_ptr(cameraUp));
		index = glGetUniformLocation(shaderId, "right");
		glUniform3fv(index, 1, glm::value_ptr(cameraRight));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, p.tex);
		
		index = glGetUniformLocation(shaderId, "pos");
		glUniform3fv(index, 1, glm::value_ptr(p.pos));

		const auto mvp = cameraMVP * glm::mat4(1.0);

		index = glGetUniformLocation(shaderId, "MVP");
		glUniformMatrix4fv(index, 1, GL_FALSE, value_ptr(mvp));


		auto t = ResourceHandler::Get().GetModel("BillBoard");
		glBindVertexArray(t->GetVAO());
		glDrawElements(t->GetType(), t->GetIndices(), GL_UNSIGNED_INT, 0);
	}
	// clear list.
	renderList.clear();
	particles.clear();
}


void GameEngine::BindMaterial(const Material* material, const int shaderID)
{
	if(material != NULL)
	{ 
		GLint idx;
		idx = glGetUniformLocation(shaderID,"mat.emissive");
		glUniform4fv(idx, 1, value_ptr(material->emissive));
		idx = idx = glGetUniformLocation(shaderID, "mat.diffuse_reflection");
		glUniform4fv(idx, 1, value_ptr(material->diffuse));
		idx = idx = glGetUniformLocation(shaderID, "mat.specular_reflection");
		glUniform4fv(idx, 1, value_ptr(material->specular));
		idx = glGetUniformLocation(shaderID, "mat.shininess");
		glUniform1f(idx, material->shininess * 128);
	}
	else
	{
		GLint idx;
		idx = glGetUniformLocation(shaderID, "mat.emissive");
		glUniform4fv(idx, 1, value_ptr(glm::vec4(0.9, 0.0, 0.7,1)));
		idx = idx = glGetUniformLocation(shaderID, "mat.diffuse_reflection");
		glUniform4fv(idx, 1, value_ptr(glm::vec4(0.9, 0.0, 0.7, 1)));
		idx = idx = glGetUniformLocation(shaderID, "mat.specular_reflection");
		glUniform4fv(idx, 1, value_ptr(glm::vec4(1, 1, 1, 1)));
		idx = glGetUniformLocation(shaderID, "mat.shininess");
		glUniform1f(idx,1);
	}
}



void GameEngine::AddToRenderList(RenderData data)
{
	// Sort vector here.
	mut.lock();
	renderList.push_back(data);
	mut.unlock();
}

void GameEngine::AddToParticleList(ParticleData particle)
{
	// Sort vector here.
	mut.lock();
	particles.push_back(particle);
	mut.unlock();
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
