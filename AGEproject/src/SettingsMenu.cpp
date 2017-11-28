#include "SettingsMenu.h"
#include "RayCast.h"
#include "GeometryUtil.h"
#include "UserControls.h"

#define CLOSE 7

int SettingsMenu::Draw(GLShader shader)
{
	const int numOfLabels = numOfTitles + numOfOptions;
	const int numOfButtons = numOfSmallButtons + numOfLargeButtons;
	int currentOption = 0;

	labels.resize(numOfLabels);
	buttons.resize(numOfButtons);
	
	// Render title labels
	for (int i = 0; i < numOfTitles; i++)
	{
		Label& newTitle = labels[i];
		newTitle.texture = title_tex[i];
		newTitle.renderTarget = Quad(glm::vec2(titleOffsetX - 1, 1 - titleOffsetY - titleHeight),
									 glm::vec2(titleOffsetX - 1 + titleWidth, 1 - titleOffsetY));
		newTitle.renderTarget.SetOpenGL();
		buttonOffsetY_Large += titleOffsetY;
		titleOffsetY += (titleHeight + changeOffset_Title);
	}

	// Render option labels
	for (int i = 0; i < numOfOptions; i++)
	{
		Label& newOption = labels[i + numOfTitles];
		newOption.texture = option_tex[i][currentOption];
		newOption.renderTarget = Quad(glm::vec2(1 - optionOffsetX - optionWidth, 1 - optionOffsetY - optionHeight),
									  glm::vec2(1 - optionOffsetX, 1 - optionOffsetY));
		newOption.renderTarget.SetOpenGL();
		optionOffsetY += (optionHeight + changeOffset_Option);
	}
	
	// Render large buttons
	for (int i = 0; i < numOfLargeButtons; i++)
	{
		Button& newLargeButton = buttons[i];
		newLargeButton.action = i;
		// Create a wider button on the first loop
		if (i == 0)
			buttonWidth_Large = 0.8f;
		else
			buttonWidth_Large = 0.6f;
		newLargeButton.renderTarget = Quad(glm::vec2(0 - (buttonWidth_Large / 2.0f), 1 - buttonOffsetY_Large - buttonHeight_Large),
									       glm::vec2(0 + (buttonWidth_Large / 2.0f), 1 - buttonOffsetY_Large));
		newLargeButton.renderTarget.SetOpenGL();
		buttonOffsetY_Large += (changeOffset_Button_Large + buttonHeight_Large);
	}

	// Render small buttons
	for (int i = 0; i < numOfSmallButtons; i++)
	{
		Button& newSmallButton = buttons[i + numOfLargeButtons];
		newSmallButton.renderTarget = Quad(glm::vec2(1 - buttonOffsetX_Small - buttonWidth_Small, 1 - buttonOffsetY_Small - buttonHeight_Small),
										   glm::vec2(1 - buttonOffsetX_Small, 1 - buttonOffsetY_Small));
		newSmallButton.renderTarget.SetOpenGL();
		if (i % 2 == 0)
		{
			newSmallButton.action = i;
			buttonOffsetX_Small += (buttonWidth_Small + changeOffset_ButtonX_Small);
		}
		else
		{
			newSmallButton.action = i;
			buttonOffsetX_Small -= (buttonWidth_Small + changeOffset_ButtonX_Small);
			buttonOffsetY_Small += (buttonHeight_Small + changeOffset_ButtonY_Small);
		}
	}

	UserControls::Get().FindConnectedJoystick();
	if (UserControls::Get().isJoystickActive() == GL_TRUE)
		currentSelection = 0;
	while (!selectionMade)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);
		shader.Use();




		if (UserControls::Get().isJoystickActive() == GL_TRUE)
		{
			timeElapsed += 1;
			int axesCount, buttonCount;
			const float * axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
			const unsigned char* keys = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
			if (timeElapsed > cooldown)
			{
				timeElapsed = 0;
				// process our joystick info
				if (GLFW_PRESS == keys[10])
				{
					SelectionUp();
				}
				if (GLFW_PRESS == keys[12])
				{
					SelectionDown();
				}
				for (int i = 0; i < buttons.size(); i++)
				{
					if (i == currentSelection)
					{
						if (i > 4)
						{
							buttons[i].texture = highlight_tex[i - 2];
						}
						else
							buttons[i].texture = highlight_tex[i];
					}
					else if (i > 4)
						buttons[i].texture = button_tex[i - 2];
					else
						buttons[i].texture = button_tex[i];
				}
				if (GLFW_PRESS == keys[0])
					selectionMade = true;
			}
		}
		else
		{
			if (!mouseButtonHeld)
			{
				// If cursor is over a button, highlight it
				for (int i = 0; i < numOfButtons; i++)
				{
					if (buttons[i].renderTarget.IsMouseInBounds())
					{
						if (i > 4)
						{
							buttons[i].texture = highlight_tex[i - 2];
						}
						else
							buttons[i].texture = highlight_tex[i];
					}
					else
					{
						if (i > 4)
							buttons[i].texture = button_tex[i - 2];
						else
							buttons[i].texture = button_tex[i];
					}
				}
			}
			selectionMade = UserControls::Get().MouseSelection(std::string("Action"), buttons, mouseButtonHeld, currentSelection);

		}
		
		// Arrow buttons.
		if (selectionMade)
		{
			if (currentSelection >= 3)
			{
				selectionMade = false;
				// Top-right arrow has been clicked
				if (currentSelection == 3)
				{
					currentOption++;
					if (currentOption > 1)
						currentOption = 0;
					labels[2].texture = option_tex[0][currentOption];
				}
				// Top-left arrow is clicked
				else if (currentSelection == 4)
				{
					currentOption--;
					if (currentOption < 0)
						currentOption = 1;
					labels[2].texture = option_tex[0][currentOption];
				}
				// Bottom-right arrow is clicked
				else if (currentSelection == 5)
				{
					currentOption++;
					if (currentOption > 1)
						currentOption = 0;
					labels[3].texture = option_tex[1][currentOption];
				}
				// Bottom-left arrow is clicked
				else if (currentSelection == 6)
				{
					currentOption--;
					if (currentOption < 0)
						currentOption = 1;
					labels[3].texture = option_tex[1][currentOption];
				}
			}
		}

		if (glfwWindowShouldClose(GameEngine::Get().GetWindow()))
		{
			selectionMade = true;
			return CLOSE;
		}


		// Draw the quads.
		for (int i = 0; i < numOfLabels; i++)
		{
			// Bind texture.
			glUniform1i(shader.GetUniformLocation("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, labels.at(i).texture);
			labels[i].renderTarget.Draw();
		}
		for (int i = 0; i < numOfButtons; i++)
		{
			// Bind texture.
			glUniform1i(shader.GetUniformLocation("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, buttons.at(i).texture);
			buttons[i].renderTarget.Draw();
		}
		glfwSwapBuffers(GameEngine::Get().GetWindow());
		glfwPollEvents();

	}
	return currentSelection;
}

void SettingsMenu::SelectionUp()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the first one, loop to end one. 
	currentSelection -= 1;
	if (currentSelection < 0)
		currentSelection = buttons.size() - 1;
}

void SettingsMenu::SelectionDown()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the last one one, loop to first one. 
	currentSelection += 1;
	if (currentSelection > buttons.size() - 1)
		currentSelection = 0;
}

void SettingsMenu::SelectionLeft()
{
}

void SettingsMenu::SelectionRight()
{
}

int SettingsMenu::SelectionPicked()
{
	return buttons.at(currentSelection).action;
}
