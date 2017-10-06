#include "Menu_Camera.h"

// Update free camera for this frame
void Menu_Camera::Update(double deltaTime)
{
	// Get current cursor position
	glfwGetCursorPos(GameEngine::Instance()->GetWindow(), &cursorX, &cursorY);
}

void Menu_Camera::from_json(const json &j) {}