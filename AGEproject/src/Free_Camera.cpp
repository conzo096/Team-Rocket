#include "Free_Camera.h"

// Update free camera (delta_time is not used)
void Free_Camera::Update(float deltaTime)
{
	// "I'd like to try and understand what's going on here a bit more." - Jack

	// Calculate the forward direction (spherical co-ordinates to Cartesian co-ordinates)
	glm::dvec3 forward(cosf(pitch) * -sinf(yaw), sinf(pitch), -cosf(yaw) * cosf(pitch));
	// Normalise forward direction
	forward = glm::normalize(forward);

	// Create standard right vector and rotate it by the yaw
	glm::dvec3 right = glm::dvec3(glm::eulerAngleY(yaw) * glm::dvec4(1.0f, 0.0f, 0.0f, 1.0f));
	// Normalise right
	right = glm::normalize(right);

	// Calculate (perpendicular) up vector using cross product
	orientation = glm::cross(right, forward);
	// Normalise up
	orientation = glm::normalize(orientation);
	
	// Update position by multiplying translation elements by forward, up and right
	glm::dvec3 trans = translation.x * right;
	trans += translation.y * orientation;
	trans += translation.z * forward;
	Move(trans);

	// Target vector is position vector plus forward
	target = GetPosition() + forward;

	// Reset the translation vector for the next frame
	translation = glm::vec3(0.0f, 0.0f, 0.0f);
	
	// Calculate view matrix
	view = glm::lookAt(GetPosition(), target, orientation);
}