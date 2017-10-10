#pragma once
#include "glm\common.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "UserControls.h"
#include "Free_Camera.h"
#include "BoundingBox.h"
class RayCast
{
	glm::vec3 origin;
	glm::vec3 direction;

	glm::vec3 near, far;
	RayCast() {}
	RayCast(glm::vec3 o, glm::vec3 dir)
	{
		origin = o;
		direction = dir;
	}

	void Get3DRayUnderMouse(Free_Camera cam)
	{
		// let's check if this renderable's AABB is clicked:
		const glm::ivec2& mCoords = UserControls::get().GetMousePos();

		int mouseY = GameEngine::Instance()->GetScreenHeight() - mCoords.y;
		GLint v[4]; //var to hold the viewport info
		glm::vec4 viewPort;

		glGetIntegerv(GL_VIEWPORT, v); //get the viewport info
		for (int i = 0; i < 4; i++)
			viewPort[i] = v[i];
		//unproject twice to build a ray from near to far plane"
		far = glm::unProject(glm::vec3(float(mCoords.x), float(mouseY), 0.0f), cam.GetView(), cam.GetProjection(), viewPort);
		near = glm::unProject(glm::vec3(float(mCoords.x), float(mouseY), 1.0f), cam.GetView(), cam.GetProjection(), viewPort);
	}

	bool RayBoxCollision(BoundingBox box)
	{
		float tmin = (box.lowerLeftFront.x - origin.x) / direction.x;
		float tmax = (box.upperRightBack.x - origin.x) / direction.x;

	
		float tymin = (box.lowerLeftFront.y - origin.y) / direction.y;
		float tymax = (box.upperRightBack.y - origin.y) / direction.y;

		
		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;

		if (tymax < tmax)
			tmax = tymax;

		float tzmin = (box.lowerLeftFront.z - origin.z) / direction.z;
		float tzmax = (box.upperRightBack.z - origin.z) / direction.z;

		
		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;

		if (tzmax < tmax)
			tmax = tzmax;

		return true;
	}   
};