#pragma once
#include "Singleton.h"
#include "GameEngine.h"
#include <map>
#include <GLFW\glfw3.h>
#include <iostream>

class UserControls : public Singleton<UserControls>
{
private:
	// If the joystick is not connected set to -1.
	int joyStickConnected = -1;

	// Map of user keys.
	std::map<std::string, unsigned int> buttonOptions;

	// Mouse position.
	double mouseX, mouseY;

	// Cursor *not implemented yet*
	GLFWcursor* cursor;


public:

	// Controller choices.
	enum ControllerOption { KEYBOARD, CONTROLLER};

	// Bind action to a new key.
	void BindKey(std::string &name, unsigned int key);

	// Returns true if it is a valid key and it is currently being pressed.
	bool IsKeyPressed(std::string &action);
	// Returns the key value of an action.
	unsigned int GetKeyValue(std::string &name)
	{
		return buttonOptions.find(name)->second;
	}

	// Search through all available index locations and connect to the first one found.
	void FindConnectedJoystick()
	{
		for (int i = 0; i < 15; i++)
		{
			if (glfwJoystickPresent(i))
			{
				joyStickConnected = i;
				break;
			}
		}
	}

	// Disconnect the joystick from the system.
	void DisconnectJoystick()
	{
		joyStickConnected = -1;
	}

	// Returns if a controller is connected or not.
	bool isJoystickActive()
	{
		if (joyStickConnected == -1)
			return false;
		return true;
	}

	// find the controller index value.
	int GetControllerIndex()
	{
		return joyStickConnected;
	}

	// Reset the keybindings to their default configuration.
	void ResetKeyBindings(ControllerOption options);

	// Change the icon of the cursor.
	bool SetCursorImage();

	// Used for colour picking to determine what object is selected. 
	int GetPickedColourIndexUnderMouse();

	// Update method.
	void Update();

	// Handle console input for debugging options (cheat menu).
	void HandleConsoleInput();
};