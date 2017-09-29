#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

// Base class for all camera types
class Camera
{
protected:
	// The camera's current position
	glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
	// The camera's current target
	glm::vec3 _target = glm::vec3(0.0f, 0.0f, -1.0f);
	// The camera's current orientation
	glm::vec3 _orientation = glm::vec3(0.0f, 1.0f, 0.0f);
	// The currently built view matrix since the last frame update
	glm::mat4 _view;
	// The currently built projection matrix since the last call to set_projection
	glm::mat4 _projection;
public:
	// Create a new camera (called by sub-classes)
	Camera() = default;
	// Destroy the camera (virtual because this is an abstract class)
	virtual ~Camera() {}
	// Get camera's current position
	const glm::vec3& get_position() const
	{
		return _position;
	}
	// Set camera's position
	void set_position(const glm::vec3 &value)
	{
		_position = value;
	}
	// Get camera's current target
	const glm::vec3& get_target() const
	{
		return _target;
	}
	// Set camera's target
	void set_target(const glm::vec3 &value)
	{
		_target = value;
	}
	// Get camera's orientation
	const glm::vec3& get_orientation() const
	{
		return _orientation;
	}
	// Set camera's orientation
	void set_orientation(const glm::vec3 &value)
	{
		_orientation = value;
	}
	// Gets camera's current view matrix
	const glm::mat4 get_view() const
	{
		return _view;
	}
	// Gets camera's current projection matrix
	glm::mat4 get_projection() const
	{
		return _projection;
	}
	// Build projection matrix
	void set_projection(float fov, float aspect, float near, float far)
	{
		_projection = glm::perspective(fov, aspect, near, far);
	}
	// Updates the camera (pure virtual function)
	virtual void update(float delta_time) = 0;
};