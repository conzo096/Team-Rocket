#include "UserControls.h"
#include "glm\gtx\string_cast.hpp"
void RayCast::UpdateRay(Free_Camera & camera)
{
	glm::vec2 mouse = UserControls::Get().GetMousePos();

	double winX = (double)mouse.x;
	double winY = GameEngine::Get().GetScreenHeight() - (double)mouse.y;

	near = glm::unProject(glm::vec3(winX, winY, 0.0), camera.GetView(), camera.GetProjection(),
		glm::vec4(0, 0, GameEngine::Get().GetScreenWidth(), GameEngine::Get().GetScreenHeight()));
	far = glm::unProject(glm::vec3(winX, winY, 1.0), camera.GetView(), camera.GetProjection(),
		glm::vec4(0, 0, GameEngine::Get().GetScreenWidth(), GameEngine::Get().GetScreenHeight()));

	origin = near;
	far.z -= 1.0f;
	direction = glm::normalize(far - origin);
}

void RayCast::UpdateRay(Menu_Camera & camera)
{
	glm::vec2 mouse = UserControls::Get().GetMousePos();

	double winX = (double)mouse.x;
	double winY = GameEngine::Get().GetScreenHeight() - (double)mouse.y;

	near = glm::unProject(glm::vec3(winX, winY, 0.0), camera.GetView(), camera.GetProjection(),
		glm::vec4(0, 0, GameEngine::Get().GetScreenWidth(), GameEngine::Get().GetScreenHeight()));
	far = glm::unProject(glm::vec3(winX, winY, 1.0), camera.GetView(), camera.GetProjection(),
		glm::vec4(0, 0, GameEngine::Get().GetScreenWidth(), GameEngine::Get().GetScreenHeight()));

	origin = near;
	//far.z -= 1.0f;
	direction = glm::normalize(far - origin);
	//std::cout << glm::to_string(direction) << std::endl;
}