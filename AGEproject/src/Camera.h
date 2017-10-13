#pragma once
#include "Entity.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

// Base class for all camera types
class Camera : public Component
{
protected:
	// The camera's current target
	glm::dvec3 target;
	// The camera's current orientation
	glm::dvec3 orientation;
	// The currently built view matrix since the last frame update
	glm::mat4 view;
	// The currently built projection matrix since the last call to set_projection
	glm::mat4 projection;
public:
	// Create a new camera (called by sub-classes)
	Camera(std::string camType) : 
		target(glm::dvec3(0.0f, 0.0f, -1.0f)), 
		orientation(glm::dvec3(0.0f, 1.0f, 0.0f)),
		view(), projection(), Component(camType) {};

	// Destroy the camera (virtual because this is an abstract class)
	virtual ~Camera() {}
	// Get camera's current target
	const glm::dvec3& GetTarget() const
	{
		return target;
	}
	// Set camera's target
	void SetTarget(const glm::dvec3 &value)
	{
		target = value;
	}
	// Get camera's orientation
	const glm::dvec3& GetOrientation() const
	{
		return orientation;
	}
	// Set camera's orientation
	void SetOrientation(const glm::dvec3 &value)
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

	// Updates the camera (pure virtual function)
	 virtual void Update(double deltaTime) override = 0 ;
};