#include "UserControls.h"

void UserControls::BindKey(std::string &name, unsigned int key)
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
		// Key is unbound. Just find it.
		buttonOptions.find(name)->second = key;
	}
	// Not match, just insert.
	else
		buttonOptions.insert(std::pair<std::string, unsigned int>(name, key));
}

bool UserControls::IsKeyPressed(std::string &action)
{
	auto val = buttonOptions.find(action);
	if (val == buttonOptions.end())
		return false;
	else
	{
		if (glfwGetKey(GameEngine::Get().GetWindow(), val->second) == GLFW_PRESS)
		{			
			return true;
		}
	}
	// Should never enter here but just in case.
	return false;
}

bool UserControls::KeyBuffer(std::string action, bool& keyHeld)
{
	if (IsKeyPressed(action))
	{
		if (!keyHeld)
		{
			keyHeld = true;
			return true;
		}
		else
			return false;
	}
	else
	{
		keyHeld = false;
		return false;
	}
}

bool UserControls::IsMouseButtonPressed(std::string &action)
{
	auto val = buttonOptions.find(action);
	if (val == buttonOptions.end())
		return false;
	else
	{
		if (glfwGetMouseButton(GameEngine::Get().GetWindow(), val->second) == GLFW_PRESS)
			return true;
	}
	// Should never enter here but just in case.
	return false;
}

bool UserControls::MouseSelection(std::string action, std::vector<Button>& buttons, bool& mouseButtonHeld, int& currentSelection)
{
	if (IsMouseButtonPressed(action))
	{
		if (!mouseButtonHeld)
		{
			for (int i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].renderTarget.IsMouseInBounds())
				{
					currentSelection = i;
					break;
				}
			}
			mouseButtonHeld = true;
		}
	}
	else
	{
		if (mouseButtonHeld)
		{
			mouseButtonHeld = false;
			if (currentSelection > -1)
			{
				if (buttons[currentSelection].renderTarget.IsMouseInBounds())
					return true;
				else
					currentSelection = -1;
			}
		}
	}
	return false;
}

bool UserControls::MouseSelection(std::string action, std::vector<std::pair<Button, UIQuad>>& buttons, bool& mouseButtonHeld, int& currentSelection)
{
	if (IsMouseButtonPressed(action))
	{
		if (!mouseButtonHeld)
		{
			for (int i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].first.renderTarget.IsMouseInBounds())
				{
					currentSelection = i;
					break;
				}
			}
			mouseButtonHeld = true;
		}
	}
	else
	{
		if (mouseButtonHeld)
		{
			mouseButtonHeld = false;
			if (currentSelection > -1)
			{
				if (buttons[currentSelection].first.renderTarget.IsMouseInBounds())
					return true;
				else
					currentSelection = -1;
			}
		}
	}
	return false;
}

void UserControls::ResetKeyBindings(ControllerOption options)
{
	// Clear the current map of keys.
	buttonOptions.clear();
	// Set up keyboard configuration.
	if (options == KEYBOARD)
	{
		// Mouse buttons and Escape (DO NOT REMAP)
		buttonOptions.insert(std::pair<std::string, unsigned int>("Action", GLFW_MOUSE_BUTTON_1));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Move", GLFW_MOUSE_BUTTON_2));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Escape", GLFW_KEY_ESCAPE));

		// Camera movement
		buttonOptions.insert(std::pair<std::string, unsigned int>("Forward", defaultKeys[0]));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Backward", defaultKeys[1]));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Left", defaultKeys[2]));
		buttonOptions.insert(std::pair<std::string, unsigned int>("Right", defaultKeys[3]));
		buttonOptions.insert(std::pair<std::string, unsigned int>("RotateLeft", defaultKeys[4]));
		buttonOptions.insert(std::pair<std::string, unsigned int>("RotateRight", defaultKeys[5]));
		buttonOptions.insert(std::pair<std::string, unsigned int>("ZoomIn", defaultKeys[7]));
		buttonOptions.insert(std::pair<std::string, unsigned int>("ZoomOut", defaultKeys[8]));

		// Hotkey/entity options. 
		buttonOptions.insert(std::pair<std::string, unsigned int>("Hold", defaultKeys[9]));
		buttonOptions.insert(std::pair<std::string, unsigned int>("HotKey1", defaultKeys[10]));
		buttonOptions.insert(std::pair<std::string, unsigned int>("HotKey2", defaultKeys[11]));
		buttonOptions.insert(std::pair<std::string, unsigned int>("HotKey3", defaultKeys[12]));
		buttonOptions.insert(std::pair<std::string, unsigned int>("HotKey4", GLFW_KEY_4));
		// Other
		buttonOptions.insert(std::pair<std::string, unsigned int>("Enter", GLFW_KEY_ENTER));
	}
	// Set up controller configuration.
	else
	{
		std::cout << "Controller not fully implemented" << std::endl;
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

bool UserControls::SetCursorImage()
{
	std::cout << "To be implemented!" << std::endl;
	/*unsigned char pixels[16 * 16 * 4];
	memset(pixels, 0xff, sizeof(pixels));
	GLFWimage image;
	image.width = 16;
	image.height = 16;
	image.pixels = pixels;
	GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);*/
	return false;

}

// This returns an value which corrosponds to the pixel value that is underneath the cursor.
// The main use is for bounding boxes in order to determine what object has been selected.
// It will only work if you render the bounding boxes before this is selected.
int UserControls::GetPickedColourIndexUnderMouse()
{
	std::cout << "Obsolete now!" << std::endl;
	//GLint viewport[4]; //var to hold the viewport info
		//GLdouble modelview[16]; //var to hold the modelview info
		//GLdouble projection[16]; //var to hold the projection matrix info
		//GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates

		//glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
		//glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
		//glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

		//winX = (float)mouseX;
		//winY = (float)viewport[3] - (float)mouseY;
		//winZ = 0;
		//GLubyte bArray[4];
		//glReadPixels(winX, winY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, bArray);
		//int iResult = (bArray[0]) | (bArray[1] << 8) | (bArray[2] << 16);
		//return iResult;
	return 0;
}

void UserControls::Update()
{
	{
		// Update cursor position.
		if(!IsJoystick)
			glfwGetCursorPos(GameEngine::Get().GetWindow(), &mouseX, &mouseY);
		else 
		{
			glfwSetCursorPos(GameEngine::Get().GetWindow(), GameEngine::Get().GetScreenWidth() / 2, GameEngine::Get().GetScreenHeight() / 2);
		}
	}
}

void UserControls::Update(Game_Camera& cam)
{
	Update();
	mouseRay.UpdateRay(cam);
}

void UserControls::Update(Free_Camera& cam)
{
	Update();
	mouseRay.UpdateRay(cam);
}

void UserControls::HandleConsoleInput()
{
	std::cout << "Still to be implemented!" << std::endl;
}



void UserControls::ResetControllerBindings()
{
	std::pair<std::string, unsigned int> temp;
	temp.first = "A";
	temp.second = 0;
	controllerButtons.insert(temp);
	temp.first = "B";
	temp.second = 1;
	controllerButtons.insert(temp);
	temp.first = "X";
	temp.second = 2;
	controllerButtons.insert(temp);
	temp.first = "Y";
	temp.second = 3;
	controllerButtons.insert(temp);
	temp.first = "leftShoulder";
	temp.second = 4;
	controllerButtons.insert(temp);
	temp.first = "rightShoulder";
	temp.second = 5;
	controllerButtons.insert(temp);
	temp.first = "back";
	temp.second = 6;
	controllerButtons.insert(temp);
	temp.first = "start";
	temp.second = 7;
	controllerButtons.insert(temp);
	temp.first = "leftSticker";
	temp.second = 8;
	controllerButtons.insert(temp);
	temp.first = "rightSticker";
	temp.second = 9;
	controllerButtons.insert(temp);
	temp.first = "dUp";
	temp.second = 10;
	controllerButtons.insert(temp);
	temp.first = "dRight";
	temp.second = 11;
	controllerButtons.insert(temp);
	temp.first = "dDown";
	temp.second = 12;
	controllerButtons.insert(temp);
	temp.first = "dLeft";
	temp.second = 13;
	controllerButtons.insert(temp);

	// Axis buttons.
	temp.first = "leftRightLeftSticker";
	temp.second = 0;
	controllerAxis.insert(temp);
	temp.first = "upDownLeftSticker";
	temp.second = 1;
	controllerAxis.insert(temp);
	temp.first = "leftRightRightSticker";
	temp.second = 2;
	controllerAxis.insert(temp);
	temp.first = "upDownRightSticker";
	temp.second = 3;
	controllerAxis.insert(temp);
	temp.first = "rightTrigger";
	temp.second = 4;
	controllerAxis.insert(temp);
	temp.first = "leftTrigger";
	temp.second = 5;

	controllerAxis.insert(temp);
}

bool UserControls::IsJoystickPressed(std::string action, ControllerAction type)
{
	ResetControllerBindings();
	isJoystickActive();
	int axesCount, buttonCount;
	const float * axes = glfwGetJoystickAxes(joyStickConnected, &axesCount);
	const unsigned char* keys = glfwGetJoystickButtons(joyStickConnected, &buttonCount);
	if (!IsJoystick)
		return false;
	if (type == BUTTON)
	{	
		if (GLFW_PRESS == keys[controllerButtons.find(action)->second])
			return true;
	}
	else
	{
		// Handle axis movement.
		//if (axes[controllerAxis.find(action)->second] > 0.7 || axes[controllerAxis.find(action)->second] < -0.7)
		//	return true;
	}
	return false;
}

float UserControls::GetAxisValue(std::string action)
{
	int axesCount;
	const float * axes = glfwGetJoystickAxes(joyStickConnected, &axesCount);
	if (!IsJoystick)
		return false;
	return axes[controllerAxis.find(action)->second];
}

std::vector<unsigned int> UserControls::GetDefaultKeys()
{
	return defaultKeys;
}

std::string UserControls::GetKeyString(const char* name)
{
	//std::cout << "Not done" << std::endl;
	std::string text("");
	unsigned int ascii = buttonOptions.find(name)->second;
	// invalid ascii character, have to handle manually.
	return AsciiToString(ascii);
}

std::string UserControls::AsciiToString(unsigned int ascii, unsigned int scanCode)
{
	const char* text;
	switch (ascii)
	{
	case -1:
		text = "Unknown";
		break;
	case 32:
		text = "Spacebar";
		break;
	case 39:
		text = "Apostrophe";
		break;
	case 259:
		text = "Backspace";
		break;
	case 341:
		text = "Left Ctrl";
		break;

	default:
		text = 	("%s",glfwGetKeyName(ascii,scanCode));
		if (text == NULL)
			text = "Dunno";
		break;
	}
	return text;
}