#include "ControlsMenu.h"
#include "UserControls.h"

#define CLOSE 14

int ControlsMenu::currentSelection = 0;
int ControlsMenu::numOfControls = 14;
std::vector <std::pair<Button, UIQuad>> ControlsMenu::buttons;
std::vector <std::string> ControlsMenu::bindings;

// Key callback method.
void ControlsMenu::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (!(currentSelection == 6 || currentSelection == 13) && currentSelection != -1)
	{
		auto keyName = glfwGetKeyName(key, scancode);
		if (keyName != NULL)
		{
			std::cout << keyName << std::endl;
			buttons[currentSelection].second.SetText(keyName);
			UserControls::Get().BindKey(bindings[currentSelection], key);
			FileIO::Get().SaveIniFile();
		}
		else
		{
			std::cout << "Need to manually type key pressed!" << std::endl;
			buttons[currentSelection].second.SetText("Invalid");
		}

		// Remove key callback.
		glfwSetKeyCallback(GameEngine::Get().GetWindow(), NULL);
	}
}

int ControlsMenu::Draw(GLShader shader)
{	
	// Initialise the quads.
	buttons.resize(numOfControls);
	// Set up binding vector.
	PopulateBindings();
	float buttonWidth = 0.46875f * 0.7f;
	float buttonHeight = 0.20926f * 0.7f;

	float buttonOffsetX = 0.2f;
	float buttonOffsetY = 0.1f;
	float offsetChange = 0.1f + buttonHeight;

	// Handle left hand side.
	for (int i = 0; i < (buttons.size()/2); i++)
	{
		Button& newButton = buttons[i].first;
		newButton.action = i;
		newButton.texture = button_tex[i];
		newButton.renderTarget = Quad(glm::vec2(-1 + buttonOffsetX, 1 - buttonOffsetY - buttonHeight),
									  glm::vec2(-1 + buttonOffsetX + buttonWidth, 1 - buttonOffsetY));
		newButton.renderTarget.SetOpenGL();
	
		if (!(i == 6))
		{
			// Draw text.
			UIQuad& u = buttons[i].second;
			u.SetText(bindings[i].c_str());
			u.SetSize(12);
			u.SetX(((buttonOffsetX + buttonWidth + 0.05f) / 2) * 800);
			u.SetY(((2 - buttonOffsetY - (buttonHeight * 0.57f)) / 2) * 600);
		}

		buttonOffsetY += offsetChange;
	}

	// Handle right hand side.
	buttonOffsetX = 0.1f;
	buttonOffsetY = 0.1f;
	for (int i = (buttons.size()/2); i < buttons.size(); i++)
	{
		Button& newButton = buttons[i].first;
		newButton.action = i;
		newButton.texture = button_tex[i];
		newButton.renderTarget = Quad(glm::vec2(0 + buttonOffsetX, 1 - buttonOffsetY - buttonHeight),
									  glm::vec2(0 + buttonOffsetX + buttonWidth, 1 - buttonOffsetY));
		newButton.renderTarget.SetOpenGL();

		if (!(i == 13))
		{
			// Draw text.
			UIQuad& u = buttons[i].second;
			u.SetText(bindings[i].c_str());
			u.SetSize(12);
			u.SetX(((1 + buttonOffsetX + buttonWidth + 0.05f) / 2) * 800);
			u.SetY(((2 - buttonOffsetY - (buttonHeight * 0.57f)) / 2) * 600);
		}

		buttonOffsetY += offsetChange;
	}

	while (!selectionMade)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);
		shader.Use();

		// Handle input.
		if (!mouseButtonHeld)
		{
			// Update textures.
			for (int i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].first.renderTarget.IsMouseInBounds())
					buttons[i].first.texture = highlight_tex[i];
				else
					buttons[i].first.texture = button_tex[i];
			}
		}

		// Handle selection
		selectionMade = UserControls::Get().MouseSelection(std::string("Action"), buttons, mouseButtonHeld, currentSelection);

		if (selectionMade)
		{
			// "Reset to default" is pressed
			if (currentSelection == 6)
			{
				selectionMade = false;
			}
			// "Back" is pressed
			else if (currentSelection == 13)
			{

			}
			else
			{
				selectionMade = false;
				glfwSetKeyCallback(GameEngine::Get().GetWindow(), KeyCallback);
			}
		}

		if (glfwWindowShouldClose(GameEngine::Get().GetWindow()))
		{
			selectionMade = true;
			return CLOSE;
		}
		
		// Draw the quads.
		for (int i = 0; i < buttons.size(); i++)
		{
			// Bind texture.
			glUniform1i(shader.GetUniformLocation("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, buttons[i].first.texture);
			buttons[i].first.renderTarget.Draw();
		}

		// Draw their corresponding bindings.
		for (int i = 0; i < buttons.size(); i++)
		{
			if (!(i == 6 || i == 13))
			{
				buttons[i].second.Render();
			}
		}
		
		glfwSwapBuffers(GameEngine::Get().GetWindow());
		glfwPollEvents();
	}
	return currentSelection;
}

void ControlsMenu::SelectionUp()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the first one, loop to end one. 
	currentSelection -= 1;
	if (currentSelection < 0)
		currentSelection = buttons.size() - 1;
}

void ControlsMenu::SelectionDown()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the last one one, loop to first one. 
	currentSelection += 1;
	if (currentSelection > buttons.size() - 1)
		currentSelection = 0;
}

void ControlsMenu::SelectionLeft()
{
}

void ControlsMenu::SelectionRight()
{
}

int ControlsMenu::SelectionPicked()
{
	return buttons.at(currentSelection).first.action;
}

void ControlsMenu::PopulateBindings()
{
	bindings.resize(numOfControls);

	// Iterate through map
	// Camera movement
	bindings[0] = "Forward";
	bindings[1] = "Backward";
	bindings[2] = "Left";
	bindings[3] = "Right";
	bindings[4] = "RotateLeft";
	bindings[5] = "RotateRight";
	bindings[6] = "Not implemented";
	bindings[7] = "ZoomIn";
	bindings[8] = "ZoomOut";

	// Hotkey/entity options
	bindings[9] = "Hold";
	bindings[10] = "HotKey1";
	bindings[11] = "HotKey2";
	bindings[12] = "HotKey3";
	bindings[13] = "Not implemented";
}