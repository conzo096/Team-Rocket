#include "SettingsMenu.h"
#include "RayCast.h"
#include "GeometryUtil.h"
#include "UserControls.h"

#define CLOSE 7

int SettingsMenu::Draw(GLShader shader)
{
	shader = *ResourceHandler::Get().GetShader("Basic");
	const int numOfLabels = numOfTitles + numOfOptions;
	const int numOfButtons = numOfSmallButtons + numOfLargeButtons;
	int currentOption1 = -1;
	int currentOption2 = -1;

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
		newOption.texture = option_tex[i][0];
		newOption.renderTarget = Quad(glm::vec2(1 - optionOffsetX - optionWidth, 1 - optionOffsetY - optionHeight),
									  glm::vec2(1 - optionOffsetX, 1 - optionOffsetY));
		newOption.renderTarget.SetOpenGL();
		optionOffsetY += (optionHeight + changeOffset_Option);
	}

	// Change option textures depending on current settings
	switch (GameEngine::Get().GetScreenHeight())
	{
	case 1080:
		labels[numOfTitles].texture = option_tex[0][0];
		currentOption1 = 0;
		break;
	case 900:
		labels[numOfTitles].texture = option_tex[0][1];
		currentOption1 = 1;
		break;
	case 720:
		labels[numOfTitles].texture = option_tex[0][2];
		currentOption1 = 2;
		break;
	case 576:
		labels[numOfTitles].texture = option_tex[0][3];
		currentOption1 = 3;
		break;
	}

	if (GameEngine::Get().GetFullScreen())
	{
		labels[numOfTitles + 1].texture = option_tex[1][1];
		currentOption2 = 1;
	}
	else
	{
		labels[numOfTitles + 1].texture = option_tex[1][0];
		currentOption2 = 0;
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
	timeElapsed = 0;
	while (!selectionMade)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);
		shader.Use();
		// Controller is being used
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

		// Keyboard is being used
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

		if (selectionMade)
		{
			// "Save changes" was clicked
			if (currentSelection == 1)
			{
				GameEngine::Get().SetFullScreen(currentOption2);
				switch (currentOption1)
				{
				case 0:
					if (!GameEngine::Get().GetFullScreen())
					{
						GameEngine::Get().SetScreenWidth(1920);
						GameEngine::Get().SetScreenHeight(1080);
					}
					GameEngine::Get().SetResolutionWidth(1920);
					GameEngine::Get().SetResolutionHeight(1080);
					break;
				case 1:
					if (!GameEngine::Get().GetFullScreen())
					{
						GameEngine::Get().SetScreenWidth(1600);
						GameEngine::Get().SetScreenHeight(900);
					}
					GameEngine::Get().SetResolutionWidth(1600);
					GameEngine::Get().SetResolutionHeight(900);
					break;
				case 2:
					if (!GameEngine::Get().GetFullScreen())
					{
						GameEngine::Get().SetScreenWidth(1280);
						GameEngine::Get().SetScreenHeight(720);
					}
					GameEngine::Get().SetResolutionWidth(1280);
					GameEngine::Get().SetResolutionHeight(720);
					break;
				case 3:
					if (!GameEngine::Get().GetFullScreen())
					{
						GameEngine::Get().SetScreenWidth(1024);
						GameEngine::Get().SetScreenHeight(576);
					}
					GameEngine::Get().SetResolutionWidth(1024);
					GameEngine::Get().SetResolutionHeight(576);
					break;
				}
				FileIO().Get().SaveIniFile();
			}
			
			// One of the option arrows was clicked
			if (currentSelection >= 3)
			{
				selectionMade = false;
				// Top-right arrow was clicked
				if (currentSelection == 3)
				{
					currentOption1++;
					if (currentOption1 > 3)
						currentOption1 = 0;
					labels[numOfTitles].texture = option_tex[0][currentOption1];
				}
				// Top-left arrow was clicked
				else if (currentSelection == 4)
				{
					currentOption1--;
					if (currentOption1 < 0)
						currentOption1 = 3;
					labels[numOfTitles].texture = option_tex[0][currentOption1];
				}
				// Bottom-right arrow was clicked
				else if (currentSelection == 5)
				{
					currentOption2++;
					if (currentOption2 > 1)
						currentOption2 = 0;
					labels[numOfTitles + 1].texture = option_tex[1][currentOption2];
				}
				// Bottom-left arrow was clicked
				else if (currentSelection == 6)
				{
					currentOption2--;
					if (currentOption2 < 0)
						currentOption2 = 1;
					labels[numOfTitles + 1].texture = option_tex[1][currentOption2];
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
			if (shader.IsLinked())
			{
				glUniform1i(shader.GetUniformLocation("tex"), 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, labels.at(i).texture);
				labels[i].renderTarget.Draw();
			}
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
