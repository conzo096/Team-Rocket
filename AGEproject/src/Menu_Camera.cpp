#include "Menu_Camera.h"

// Update free camera for this frame
void Menu_Camera::Update(double deltaTime)
{
	glfwSetInputMode(GameEngine::Get().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Get current cursor position
	glfwGetCursorPos(GameEngine::Get().GetWindow(), &cursorX, &cursorY);

	// Calculate forward, right and up vectors
	glm::dvec3 forward = target - GetPosition();
	glm::dvec3 right = glm::cross(GetOrientation(), forward);
	glm::dvec3 up = glm::normalize(glm::cross(forward, right));

	// Calculate view matrix
	view = glm::lookAt(GetPosition(), target, up);
}

void Menu_Camera::from_json(const json &j) {}