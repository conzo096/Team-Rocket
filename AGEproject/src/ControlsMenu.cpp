#include "ControlsMenu.h"
#include "UserControls.h"

#define CLOSE 14

int ControlsMenu::currentSelection = -1;
int ControlsMenu::lastSelection = -1;
int const ControlsMenu::numOfControls = 14;
std::vector <std::pair<Button, UIQuad>> ControlsMenu::buttons;
std::vector <std::string> ControlsMenu::bindings;
std::vector <unsigned int> ControlsMenu::button_tex;
std::vector <unsigned int> ControlsMenu::highlight_tex;
std::vector <unsigned int> ControlsMenu::current_tex;

// Key callback method.
void ControlsMenu::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (!(currentSelection == 6 || currentSelection == 13) && currentSelection != -1)
	{
		buttons[currentSelection].second.SetText(UserControls::Get().AsciiToString(key,scancode).c_str());
		UserControls::Get().BindKey(bindings[currentSelection], key);
		FileIO::Get().SaveIniFile();

		current_tex[currentSelection] = button_tex[currentSelection];

		// Remove key callback.
		glfwSetKeyCallback(GameEngine::Get().GetWindow(), NULL);
	}
}

void ControlsMenu::DrawButtons()
{
	// Initialise the quads.
	buttons.resize(numOfControls);
	// Set up binding vector.
	PopulateBindings();
	float buttonWidth = 0.46875f * 0.7f;
	float buttonHeight = 0.20926f * 0.7f;
	float resetWidth = 0.36458f * 0.7f;
	float resetHeight = 0.27778f * 0.7f;

	float buttonOffsetX = 0.2f;
	float buttonOffsetY = 0.1f;
	float offsetChange = 0.1f + buttonHeight;

	// Handle left hand side.
	for (int i = 0; i < (buttons.size() / 2); i++)
	{
		Button& newButton = buttons[i].first;
		newButton.action = i;
		newButton.texture = current_tex[i];

		if (!(i == 6))
		{
			newButton.renderTarget = Quad(glm::vec2(-1 + buttonOffsetX, 1 - buttonOffsetY - buttonHeight),
				glm::vec2(-1 + buttonOffsetX + buttonWidth, 1 - buttonOffsetY));

			// Draw text.
			UIQuad& u = buttons[i].second;
			u.SetText(bindings[i].c_str());
			u.SetSize(12);
			u.SetX(((buttonOffsetX + buttonWidth + 0.05f) / 2) * 800);
			u.SetY(((2 - buttonOffsetY - (buttonHeight * 0.57f)) / 2) * 600);

			buttonOffsetY += offsetChange;
		}
		else
		{
			buttonOffsetX = 0.05f;
			buttonOffsetY += 0.1f;
			newButton.renderTarget = Quad(glm::vec2(0 - buttonOffsetX - resetWidth, 1 - buttonOffsetY - resetHeight),
				glm::vec2(0 - buttonOffsetX, 1 - buttonOffsetY));
		}
		newButton.renderTarget.SetOpenGL();
	}

	// Handle right hand side.
	buttonOffsetX = 0.1f;
	buttonOffsetY = 0.1f;
	for (int i = (buttons.size() / 2); i < buttons.size(); i++)
	{
		Button& newButton = buttons[i].first;
		newButton.action = i;
		newButton.texture = current_tex[i];

		if (!(i == 13))
		{
			newButton.renderTarget = Quad(glm::vec2(0 + buttonOffsetX, 1 - buttonOffsetY - buttonHeight),
										  glm::vec2(0 + buttonOffsetX + buttonWidth, 1 - buttonOffsetY));

			// Draw text.
			UIQuad& u = buttons[i].second;
			u.SetText(bindings[i].c_str());
			u.SetSize(12);
			u.SetX(((1 + buttonOffsetX + buttonWidth + 0.05f) / 2) * 800);
			u.SetY(((2 - buttonOffsetY - (buttonHeight * 0.57f)) / 2) * 600);

			buttonOffsetY += offsetChange;
		}
		else
		{
			buttonOffsetX = 0.05f;
			buttonOffsetY += 0.1f;
			newButton.renderTarget = Quad(glm::vec2(0 + buttonOffsetX, 1 - buttonOffsetY - resetHeight),
										  glm::vec2(0 + buttonOffsetX + resetWidth, 1 - buttonOffsetY));
		}
		newButton.renderTarget.SetOpenGL();
	}
}

int ControlsMenu::Draw(GLShader shader)
{	
	DrawButtons();

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
					buttons[i].first.texture = current_tex[i];
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
				UserControls::Get().ResetKeyBindings(UserControls::Get().KEYBOARD);

				for (int i = 0; i < buttons.size(); i++)
				{
					if (!(i == 6 || i == 13))
					{
						UserControls::Get().BindKey(bindings[i], UserControls::Get().GetDefaultKeys()[i]);
						buttons[i].second.SetText(UserControls::Get().AsciiToString(UserControls::Get().GetDefaultKeys()[i]).c_str());
						current_tex[i] = button_tex[i];
					}
				}
				FileIO::Get().SaveIniFile();
			}
			// "Back" is pressed
			else if (currentSelection == 13)
			{
				FileIO::Get().SaveIniFile();
			}
			else
			{
				selectionMade = false;
				if (lastSelection == currentSelection)
				{
					if (current_tex[currentSelection] == highlight_tex[currentSelection])
					{
						current_tex[currentSelection] = button_tex[currentSelection];
					}
					else if (current_tex[currentSelection] == button_tex[currentSelection])
					{
						current_tex[currentSelection] = highlight_tex[currentSelection];
						glfwSetKeyCallback(GameEngine::Get().GetWindow(), KeyCallback);
					}
					lastSelection = currentSelection;
				}
				else
				{
					current_tex[currentSelection] = highlight_tex[currentSelection];
					glfwSetKeyCallback(GameEngine::Get().GetWindow(), KeyCallback);
					if (!(lastSelection == -1))
					{
						current_tex[lastSelection] = button_tex[lastSelection];
					}
					lastSelection = currentSelection;
				}
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
			if (i != 6 && i != 13)
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
	bindings[0] = UserControls::Get().GetKeyString("Forward");// "Forward";
	bindings[1] = UserControls::Get().GetKeyString("Backward"); //"Backward";
	bindings[2] = UserControls::Get().GetKeyString("Left");//"Left";
	bindings[3] = UserControls::Get().GetKeyString("Right"); //"Right";
	bindings[4] = UserControls::Get().GetKeyString("RotateLeft"); //"RotateLeft";
	bindings[5] = UserControls::Get().GetKeyString("RotateRight"); //"RotateRight";
	bindings[6] = "Not implemented";
	bindings[7] = UserControls::Get().GetKeyString("ZoomIn"); //"ZoomIn";
	bindings[8] = UserControls::Get().GetKeyString("ZoomOut"); //"ZoomOut";

	// Hotkey/entity options
	bindings[9] = UserControls::Get().GetKeyString("Hold"); //"Hold";
	bindings[10] = UserControls::Get().GetKeyString("HotKey1"); //"HotKey1";
	bindings[11] = UserControls::Get().GetKeyString("HotKey2");//"HotKey2";
	bindings[12] = UserControls::Get().GetKeyString("HotKey3");//"HotKey3";
	bindings[13] = "Not implemented";
}