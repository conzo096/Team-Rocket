#pragma once
#include "glm\common.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "UserControls.h"
#include "Free_Camera.h"



class RayCast
{
public:
	// Ray is just a position and a direction.
	glm::vec3 origin;
	glm::vec3 direction;

	RayCast() {}

	RayCast(Free_Camera& camera)
	{
		
		glm::vec2 mouse = UserControls::get().GetMousePos();

		//mouse.x = mouse.x * (2 / GameEngine::Instance()->GetScreenWidth()) - 1;
		//mouse.y = mouse.y * (2 / GameEngine::Instance()->GetScreenHeight()) - 1;

		//// Near plane.
		//glm::vec3 v0 = glm::unProject(glm::vec3(float(mouse.x), float(mouse.y), 0.0f), camera.GetView(), camera.GetProjection(), glm::vec4(0,0,GameEngine::Instance()->GetScreenWidth(), GameEngine::Instance()->GetScreenHeight()));
		//// far plane.
		//glm::vec3 v1 = glm::unProject(glm::vec3(float(mouse.x), float(mouse.y), 1.0f), camera.GetView(), camera.GetProjection(), glm::vec4(0, 0, GameEngine::Instance()->GetScreenWidth(), GameEngine::Instance()->GetScreenHeight()));

		//// Calculate direction.
		//direction = (v1 - v0);
		//// Set origin to camera position.
		//origin = camera.GetPosition();


		float x = (2.0f * mouse.x) / GameEngine::Instance()->GetScreenWidth() - 1.0f;
		float y = 1.0f - (2.0f * mouse.y) / GameEngine::Instance()->GetScreenHeight();
		float z = 1.0f;
		glm::vec3 ray_nds = glm::vec3(x, y, z);
		glm::vec4 ray_clip = glm::vec4(glm::vec2(ray_nds), -1.0, 1.0);
		glm::vec4 ray_eye = glm::inverse(camera.GetProjection()) * ray_clip;
		ray_eye = glm::vec4(glm::vec2(ray_eye), -1.0, 0.0);
		glm::vec3 ray_wor = (glm::inverse(camera.GetView()) * ray_eye);
		// don't forget to normalise the vector at some point
		direction = glm::normalize(ray_wor);
		origin = camera.GetPosition();
	}
	RayCast(glm::vec3 o, glm::vec3 dir)
	{
		origin = o;
		direction = dir;
	}



};