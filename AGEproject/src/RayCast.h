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
	glm::vec3 near;
	glm::vec3 far;
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


		//float x = (2.0f * mouse.x) / GameEngine::Instance()->GetScreenWidth() - 1.0f;
		//float y = 1.0f - (2.0f * mouse.y) / GameEngine::Instance()->GetScreenHeight();
		//float z = 1.0f;
		//glm::vec3 ray_nds = glm::vec3(x, y, z);
		//glm::vec4 ray_clip = glm::vec4(glm::vec2(ray_nds), -1.0, 1.0);
		//glm::vec4 ray_eye = glm::inverse(camera.GetProjection()) * ray_clip;
		//ray_eye = glm::vec4(glm::vec2(ray_eye), -1.0, 0.0);
		//glm::vec3 ray_wor = (glm::inverse(camera.GetView()) * ray_eye);
		//// don't forget to normalise the vector at some point
		//direction = glm::normalize(ray_wor);
		//origin = camera.GetPosition();

	/*	double winX = (double)mouse.x;
		double winY = GameEngine::Instance()->GetScreenHeight() - (double)mouse.y;


		near = glm ::unProject(glm::vec3(winX, winY, 0.0), camera.GetView(), camera.GetProjection(),
			glm::vec4(0,0,GameEngine::Instance()->GetScreenWidth(),GameEngine::Instance()->GetScreenHeight()));
		far = glm::unProject(glm::vec3(winX, winY, 1.0), camera.GetView(), camera.GetProjection(),
			glm::vec4(0, 0, GameEngine::Instance()->GetScreenWidth(), GameEngine::Instance()->GetScreenHeight()));

		origin = near;
		direction = glm::normalize(far - near);
*/

		int mouseX = mouse.x;
		int mouseY = mouse.y; 
		int screenWidth = GameEngine::Instance()->GetScreenWidth();
		int screenHeight = GameEngine::Instance()->GetScreenHeight();
		glm::mat4 ViewMatrix = camera.GetView();
		glm::mat4 ProjectionMatrix = camera.GetProjection();

		// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
		glm::vec4 lRayStart_NDC
		(
			((float)mouseX / (float)screenWidth - 0.5f) * 2.0f, 
			((float)mouseY / (float)screenHeight - 0.5f) * 2.0f,
			-1.0,1.0f
		);
		glm::vec4 lRayEnd_NDC
		(
			((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,
			((float)mouseY / (float)screenHeight - 0.5f) * 2.0f,
			0.0,1.0f
		);
		// The Projection matrix goes from Camera Space to NDC.
		// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
		glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

		// The View Matrix goes from World Space to Camera Space.
		// So inverse(ViewMatrix) goes from Camera Space to World Space.
		glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

		glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
		glm::vec4 lRayStart_world = InverseViewMatrix       * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
		glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
		glm::vec4 lRayEnd_world = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;


		// Faster way (just one inverse)
		//glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
		//glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
		//glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


		glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
		lRayDir_world = glm::normalize(lRayDir_world);


		origin = glm::vec3(lRayStart_world);
		direction = glm::normalize(lRayDir_world);


	}
	RayCast(glm::vec3 o, glm::vec3 dir)
	{
		origin = o;
		direction = dir;
	}



};