#pragma once
#include "Singleton.h"
#include "GameEngine.h"
#include "RayCast.h"
#include "Menu.h"
#include "UIQuad.h"
#include "Game_Camera.h"
#include "Free_Camera.h"
#include <map>
#include <GLFW\glfw3.h>
#include <iostream>

class UserControls : public Singleton<UserControls>
{
private:
	// If the joystick is not connected set to -1.
	int joyStickConnected = -1;

	// Map of keyboard keys.
	std::map<std::string, unsigned int> buttonOptions;
	// Map of controller keys.
	std::map<std::string, unsigned int> controllerButtons;
	// Map of controller axis.
	std::map<std::string, unsigned int> controllerAxis;

	// List of default keys
	std::vector<unsigned int> defaultKeys =
		{ GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_Q, GLFW_KEY_E, 0,
		  GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_X, GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, 0 };

	// Mouse position.
	double mouseX, mouseY;

	// Cursor *not implemented yet*
	GLFWcursor* cursor;

	// Ray mouse generates.
	RayCast mouseRay;

	bool IsJoystick = false;

public:
	// Controller choices.
	enum ControllerOption { KEYBOARD, CONTROLLER };
	enum ControllerAction { BUTTON, AXIS };
	RayCast GetRay() { return mouseRay; };

	// Bind action to a new key.
	void BindKey(std::string &name, unsigned int key);

	// Returns true if it is a valid key and it is currently being pressed.
	bool IsKeyPressed(std::string &action);

	bool KeyBuffer(std::string action, bool& keyHeld);

	bool IsMouseButtonPressed(std::string &action);

	bool MouseSelection(std::string action, std::vector<Button>& buttons, bool& mouseButtonHeld, int& currentSelection);
	bool MouseSelection(std::string action, std::vector<std::pair<Button, UIQuad>>& buttons, bool& mouseButtonHeld, int& currentSelection);

	// Returns the key value of an action.
	unsigned int GetKeyValue(std::string &name)
	{
		return buttonOptions.find(name)->second;
	}

	unsigned int GetKeyValue(const char* name)
	{
		return buttonOptions.find(name)->second;
	}
	std::string GetKeyString(const char* name);
	std::string UserControls::AsciiToString(unsigned int ascii,unsigned int scanCode = 0);
	// Search through all available index locations and connect to the first one found.
	void FindConnectedJoystick()
	{
		for (int i = 0; i < 15; i++)
		{
			if (glfwJoystickPresent(i))
			{
				IsJoystick = true;
				joyStickConnected = i;
				break;
			}
		}
	}

	// Disconnect the joystick from the system.
	void DisconnectJoystick()
	{
		joyStickConnected = -1;
		IsJoystick = false;
	}

	// Returns if a controller is connected or not.
	bool isJoystickActive()
	{
		FindConnectedJoystick();
		if (glfwJoystickPresent(joyStickConnected) == false)
		{
			IsJoystick = false;
			return false;
		}
		IsJoystick = true;
		return true;
	}

	// find the controller index value.
	int GetControllerIndex()
	{
		return joyStickConnected;
	}

	bool IsJoyStickConnected()
	{
		return IsJoystick;
	}


	// Returns mouse position.
	glm::ivec2 GetMousePos()
	{
		glfwGetCursorPos(GameEngine::Get().GetWindow(), &mouseX, &mouseY);
		return glm::ivec2(mouseX, mouseY);
	}


	// Obtain map list.
	std::map<std::string,unsigned int> GetMapBindings()&
	{
		return buttonOptions;
	}


	// Reset the keybindings to their default configuration.
	void ResetKeyBindings(ControllerOption options);

	// Change the icon of the cursor.
	bool SetCursorImage();

	// Used for colour picking to determine what object is selected. 
	int GetPickedColourIndexUnderMouse();

	// Update method.
	void Update();
	void Update(Game_Camera& camera);
	void Update(Free_Camera& camera);
	// Handle console input for debugging options (cheat menu).
	void HandleConsoleInput();

	void ResetControllerBindings();
	bool IsJoystickPressed(std::string action, ControllerAction type = BUTTON);



	float GetAxisValue(std::string action);
	void BindControllerButton(std::string &name, unsigned int key);
	std::string UserControls::GetButtonString(const char* name);
	std::string UserControls::GetButtonString(int name);
	std::vector<unsigned int> GetDefaultKeys();
};