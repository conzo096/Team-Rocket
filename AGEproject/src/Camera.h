#pragma once
#include "Entity.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

// Base class for all camera types
class Camera : public Component
{
protected:
	// The camera's current target
	glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f);
	// The camera's current orientation
	glm::vec3 orientation = glm::vec3(0.0f, 1.0f, 0.0f);
	// The currently built view matrix since the last frame update
	glm::mat4 view;
	// The currently built projection matrix since the last call to set_projection
	glm::mat4 projection;
public:
	// Create a new camera (called by sub-classes)
	Camera() : Component("Camera") {};
	// Destroy the camera (virtual because this is an abstract class)
	virtual ~Camera() {}
	// Get camera's current target
	const glm::vec3& GetTarget() const
	{
		return target;
	}
	// Set camera's target
	void SetTarget(const glm::vec3 &value)
	{
		target = value;
	}
	// Get camera's orientation
	const glm::vec3& GetOrientation() const
	{
		return orientation;
	}
	// Set camera's orientation
	void SetOrientation(const glm::vec3 &value)
	{
		orientation = value;
	}
	// Gets camera's current view matrix
	const glm::mat4 GetView() const
	{
		return view;
	}
	// Gets camera's current projection matrix
	glm::mat4 GetProjection() const
	{
		return projection;
	}
	// Build projection matrix
	void SetProjection(float fov, float aspect, float near, float far)
	{
		projection = glm::perspective(fov, aspect, near, far);
	}
	// Updates the camera (pure virtual function)
	virtual void update(float delta_time) = 0;
};