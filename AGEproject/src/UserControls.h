#pragma once
#include <map>
#include <GLFW\glfw3.h>
#include "Singleton.h"
class UserControls : Singleton<UserControls>
{
public:
	// Controller choices.
	enum ControllerOption { KEYBOARD, CONTROLLER};
	// User interaction option.
	
	// If the joystick is not connected set to -1.
	int joyStickConnected = -1;

	// Map of user keys.
	std::map<std::string, unsigned int> buttonOptions;

	// Mouse position.
	int mouseX, mouseY;


	void BindKey(std::string name,  int key)
	{
		// If action has a binding...
		if (buttonOptions.find(name) != buttonOptions.end())
		{
			// If new key is being used for something else, swap them.
			for (auto it = buttonOptions.begin(); it != buttonOptions.end(); ++it)
			{
				if (it->second == key)
				{
					// There is a match, swap and break.
					int valueToSwap = buttonOptions.find(name)->second;
					buttonOptions.find(name)->second = it->second;
					it->second = valueToSwap;
					return;
				}
			}
		}
		// Not match, just insert.
		else
			buttonOptions.insert(std::pair<std::string,unsigned int>(name, key));
	}

	unsigned int GetKey(std::string name)
	{
		return buttonOptions.find(name)->second;
	}

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

	int GetControllerIndex()
	{
		return joyStickConnected;
	}

	void ResetKeyBindings(ControllerOption options)
	{
		buttonOptions.clear();
		// Set up keyboard configuration.
		if (options == KEYBOARD)
		{
			buttonOptions.insert(std::pair<std::string, unsigned int>("Action", GLFW_MOUSE_BUTTON_1));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Move forward", GLFW_KEY_W));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Move backward", GLFW_KEY_S));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Move left", GLFW_KEY_A));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Move right", GLFW_KEY_D));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Reset camera",GLFW_KEY_SPACE));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Escape", GLFW_KEY_ESCAPE));

		}
		// Set up controller configuration.
		else
		{
			// I got the values for the controls here:
			// https://github.com/LWJGL/lwjgl3-wiki/wiki/2.6.3-Input-handling-with-GLFW
			buttonOptions.insert(std::pair<std::string, unsigned int>("Action", 0));
			//buttonOptions.insert(std::pair<std::string, unsigned int>("Move Forward", GLFW_KEY_W));
			//buttonOptions.insert(std::pair<std::string, unsigned int>("Move Backward", GLFW_KEY_S));
			//buttonOptions.insert(std::pair<std::string, unsigned int>("Move left", GLFW_KEY_A));
			//buttonOptions.insert(std::pair<std::string, unsigned int>("Move right", GLFW_KEY_D));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Reset camera", 10));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Escape", 7));

		}
	}





};