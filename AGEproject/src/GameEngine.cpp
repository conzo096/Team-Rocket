#include "GameEngine.h"
#include <assert.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "FileIO.h"
#include "UserControls.h"
#include <tuple>
#include "AudioEngine.h"
#include "PointLight.h"


void GameEngine::AddPointLight(PointLight* light)
{ 
	lights.push_back(light);
}
std::vector<PointLight*>& GameEngine::GetPointLights() 
{
	return lights;
}

void GameEngine::Initialise()
{
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: glfw failed init! exiting.");
		return;
	}
	std::cout << "Creating or loading config file" << std::endl;
	FileIO io = FileIO::Get();
	io.LoadIniFile();

	std::cout << "Creating game window" << std::endl;
	GameEngine::CreateWindow();
	
	// Window is now initalised, now make it the current context.
	glfwMakeContextCurrent(window);

	if (!Get().window)
	{
		assert(Get().window != NULL);
		fprintf(stderr, "Window could not be initialised!");
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
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// V-Sync, does not run without it
	glfwSwapInterval(1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	AudioEngine::Get().Initialise();


}


void GameEngine::Render()
{
	glClearColor(0.1f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	for (int i = 0; i < renderList.size(); i++)
	{
		RenderData& rl = renderList[i];
		// Bind shader.
		glUseProgram(rl.shader);
		// Bind Uniforms.
		const auto mvp = cameraMVP * rl.m;
		// Bind all the lights.
		for (int i = 0; i < lights.size(); i++)
			lights[i]->Render();
		GLint index;
		index = glGetUniformLocation(rl.shader, "MVP");
		glUniformMatrix4fv(index, 1, GL_FALSE, value_ptr(mvp));
		index = glGetUniformLocation(rl.shader, "M");
		glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(rl.m));
		index = glGetUniformLocation(rl.shader, "N");
		glUniformMatrix3fv(index, 1, GL_FALSE, glm::value_ptr(glm::mat3(rl.m)));
		index = glGetUniformLocation(rl.shader, "eye_pos");
		glm::vec3 eyePos = glm::vec3(mvp[0][3], mvp[1][3], mvp[2][3]);
		glUniform3fv(index, 1, glm::value_ptr(eyePos));
		index = glGetUniformLocation(rl.shader, "highlightColour");
		glUniform4fv(index,1,glm::value_ptr(rl.highlightColour));
		// Bind material.
		BindMaterial(rl.mat, rl.shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rl.texture);

		// Bind and draw model.
		glBindVertexArray(rl.modelVao);
		glDrawElements(rl.drawType, rl.indices, GL_UNSIGNED_INT, 0);
		if (i < renderList.size()-1)
		{
			if(renderList[i+1].modelVao != renderList[i].modelVao)
				glBindVertexArray(0);
		}
		else
			glBindVertexArray(0);
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
}

void GameEngine::CreateWindow()
{
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	if (GameEngine::Get().GetFullScreen())
	{
		window = glfwCreateWindow(GameEngine::Get().GetScreenWidth(), GameEngine::Get().GetScreenHeight(), "Scrapitalism", glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		window = glfwCreateWindow(GameEngine::Get().GetScreenWidth(), GameEngine::Get().GetScreenHeight(), "Scrapitalism", NULL, NULL);
	}
	glfwMakeContextCurrent(window);
}

void GameEngine::UpdateWindow()
{
	if (!fullScreen)
	{
		glfwSetWindowMonitor(GameEngine::Get().GetWindow(), NULL, 20, 50, GameEngine::Get().GetScreenWidth(), GameEngine::Get().GetScreenHeight(), 60);
	}
	else
	{
		glfwSetWindowMonitor(GameEngine::Get().GetWindow(), glfwGetPrimaryMonitor(), 0, 0, GameEngine::Get().GetResolutionWidth(), GameEngine::Get().GetResolutionHeight(), 60);
	}
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);
	SetScreenWidth(w);
	SetScreenHeight(h);
	glfwMakeContextCurrent(window);
}

void GameEngine::AddToRenderList(RenderData data)
{
	// Sort vector here.
	mut.lock();
	if (data.sphereRadius == 0)
	{
		renderList.push_back(data);
		// Lazy sort - sorts renderlist by shader id then type of model. Would be smarter by calculate where 
		// it should be inserted to first.
		std::sort(renderList.begin(), renderList.end(), [](const RenderData& lhs, const RenderData& rhs)
		{
			return std::tie(lhs.shader, lhs.modelVao, lhs.drawType) < std::tie(rhs.shader, rhs.modelVao,lhs.drawType);
		});
	}
	else if(IsInCameraFrustum(data))
	{
		renderList.push_back(data);
		// Lazy sort - sorts renderlist by shader id then type of model. Would be smarter by calculate where 
		// it should be inserted to first.

		std::sort(renderList.begin(), renderList.end(), [](const RenderData& lhs, const RenderData& rhs)
		{
			return std::tie(lhs.shader, lhs.modelVao, lhs.drawType) < std::tie(rhs.shader, rhs.modelVao, lhs.drawType);
		});
	}
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
	GenerateFrustumPlanes();
}

void GameEngine::CleanUp()
{
	AudioEngine::Get().Shutdown();
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

void GameEngine::GenerateFrustumPlanes()
{
	// Left plane
	frustumPlanes[0].x = cameraMVP[0][3] + cameraMVP[0][0];
	frustumPlanes[0].y = cameraMVP[1][3] + cameraMVP[1][0];
	frustumPlanes[0].z = cameraMVP[2][3] + cameraMVP[2][0];
	frustumPlanes[0].w = cameraMVP[3][3] + cameraMVP[3][0];

	// Right plane
	frustumPlanes[1].x = cameraMVP[0][3] - cameraMVP[0][0];
	frustumPlanes[1].y = cameraMVP[1][3] - cameraMVP[1][0];
	frustumPlanes[1].z = cameraMVP[2][3] - cameraMVP[2][0];
	frustumPlanes[1].w = cameraMVP[3][3] - cameraMVP[3][0];

	// Top plane
	frustumPlanes[2].x = cameraMVP[0][3] - cameraMVP[0][1];
	frustumPlanes[2].y = cameraMVP[1][3] - cameraMVP[1][1];
	frustumPlanes[2].z = cameraMVP[2][3] - cameraMVP[2][1];
	frustumPlanes[2].w = cameraMVP[3][3] - cameraMVP[3][1];

	// Bottom plane
	frustumPlanes[3].x = cameraMVP[0][3] + cameraMVP[0][1];
	frustumPlanes[3].y = cameraMVP[1][3] + cameraMVP[1][1];
	frustumPlanes[3].z = cameraMVP[2][3] + cameraMVP[2][1];
	frustumPlanes[3].w = cameraMVP[3][3] + cameraMVP[3][1];

	// Near plane
	frustumPlanes[4].x = cameraMVP[0][2] + cameraMVP[0][1];
	frustumPlanes[4].y = cameraMVP[1][2] + cameraMVP[1][2];
	frustumPlanes[4].z = cameraMVP[2][2] + cameraMVP[2][2];
	frustumPlanes[4].w = cameraMVP[3][2] + cameraMVP[3][2];

	// Far plane
	frustumPlanes[5].x = cameraMVP[0][3] - cameraMVP[0][2];
	frustumPlanes[5].y = cameraMVP[1][3] - cameraMVP[1][2];
	frustumPlanes[5].z = cameraMVP[2][3] - cameraMVP[2][2];
	frustumPlanes[5].w = cameraMVP[3][3] - cameraMVP[3][2];

	// Normalize planes
	for (int i = 0; i < 6; i++)
	{	
		frustumPlanes[i] =	glm::normalize(frustumPlanes[i]);
	}

}

bool GameEngine::IsInCameraFrustum(RenderData& rd)
{

	// Not fully tested.
	//for (int i = 0; i < 6; i++)
	//{
	//	if ((glm::dot(frustumPlanes[i], glm::vec4(rd.boundingPoint, 1)) + rd.sphereRadius) < 0)
	//	{
	//		std::cout << i << std::endl;
	//		return false;
	//	}
	//}
	//return true;

	// Check if the radius of the sphere is inside the view frustum.
	for (int i = 0; i < 6; i++)
	{
		if (glm::dot(frustumPlanes[i], glm::vec4(rd.boundingPoint,1)) < -1)
		{
			return false;
		}
	}
	return true;
}


