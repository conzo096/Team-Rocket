#pragma once
#include <map>
#include <GLFW\glfw3.h>
#include "Singleton.h"
#include "GameEngine.h"
#include <thread>
#include <iostream>
#include <future>
#include <conio.h>

// Handle console input.
std::string HandleConsoleInput()
{
	std::string input;

	if (!_kbhit())
		return "Nothing";
	else
		std::cin >> input;
	return input;
}


class UserControls : Singleton<UserControls>
{
public:
	std::future<std::string> info = std::async(std::launch::async, HandleConsoleInput);;
	// Controller choices.
	enum ControllerOption { KEYBOARD, CONTROLLER};
	// User interaction option.
	
	// If the joystick is not connected set to -1.
	int joyStickConnected = -1;

	// Map of user keys.
	std::map<std::string, unsigned int> buttonOptions;

	// Mouse position.
	double mouseX, mouseY;

	GLFWcursor* cursor;

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

	void UpdateMousePosition()
	{
		glfwGetCursorPos(GameEngine::Instance()->GetWindow(), &mouseX, &mouseY);
	}

	void ResetKeyBindings(ControllerOption options)
	{
		buttonOptions.clear();
		// Set up keyboard configuration.
		if (options == KEYBOARD)
		{
			buttonOptions.insert(std::pair<std::string, unsigned int>("Action", GLFW_MOUSE_BUTTON_1));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Forward", GLFW_KEY_W));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Backward", GLFW_KEY_S));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Left", GLFW_KEY_A));
			buttonOptions.insert(std::pair<std::string, unsigned int>("Right", GLFW_KEY_D));
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

	bool SetCursorImage()
	{
		unsigned char pixels[16 * 16 * 4];
		memset(pixels, 0xff, sizeof(pixels));
		GLFWimage image;
		image.width = 16;
		image.height = 16;
		image.pixels = pixels;
		GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
	}


	// Used for colour picking to determine what object is selected. 
	int GetPickedColorIndexUnderMouse()
	{
		GLint viewport[4]; //var to hold the viewport info
		GLdouble modelview[16]; //var to hold the modelview info
		GLdouble projection[16]; //var to hold the projection matrix info
		GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
		GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

		glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
		glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
		glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

		winX = (float)mouseX;
		winY = (float)viewport[3] - (float)mouseY;
		winZ = 0;
		GLubyte bArray[4];
		glReadPixels(winX, winY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, bArray);
		int iResult = (bArray[0]) | (bArray[1] << 8) | (bArray[2] << 16);
		return iResult;
	}


	void Update()
	{
		if (info.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			std::string para = info.get();
			if (para.compare("Nothing"))
				std::cout << "Got: " << para << std::endl;;
			info = std::async(std::launch::async, HandleConsoleInput);
		}
		else
		{
		//	std::cout << "waiting..." << std::endl;
			return;
		}
	}
};