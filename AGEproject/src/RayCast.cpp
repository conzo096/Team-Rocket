#include "RayCast.h"
#include "UserControls.h"
void RayCast::UpdateRay(Free_Camera & camera)
{
	glm::vec2 mouse = UserControls::get().GetMousePos();

	double winX = (double)mouse.x;
	double winY = GameEngine::Instance()->GetScreenHeight() - (double)mouse.y;

	near = glm::unProject(glm::vec3(winX, winY, 0.0), camera.GetView(), camera.GetProjection(),
		glm::vec4(0, 0, GameEngine::Instance()->GetScreenWidth(), GameEngine::Instance()->GetScreenHeight()));
	far = glm::unProject(glm::vec3(winX, winY, 1.0), camera.GetView(), camera.GetProjection(),
		glm::vec4(0, 0, GameEngine::Instance()->GetScreenWidth(), GameEngine::Instance()->GetScreenHeight()));

	origin = near;
	far.z -= 1.0f;
	direction = glm::normalize(far - origin);
}
