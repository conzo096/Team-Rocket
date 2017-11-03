#include "SettingsMenu.h"
#include "RayCast.h"
#include "GeometryUtil.h"

#define CLOSE 8

int SettingsMenu::Draw(GLShader shader)
{
	const int numOfLabels = numOfTitles + numOfOptions;
	const int numOfButtons = numOfSmallButtons + numOfLargeButtons;

	labels.resize(numOfLabels);
	buttons.resize(numOfButtons);
	
	// Render title labels
	for (int i = 0; i < numOfTitles; i++)
	{
		Label& newTitle = labels[i];
		newTitle.texture = tex;
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
		newOption.texture = tex;
		newOption.renderTarget = Quad(glm::vec2(1 - optionOffsetX - optionWidth, 1 - optionOffsetY - optionHeight),
									  glm::vec2(1 - optionOffsetX, 1 - optionOffsetY));
		newOption.renderTarget.SetOpenGL();
		optionOffsetY += (optionHeight + changeOffset_Option);
	}
	
	// Render large buttons
	for (int i = 0; i < numOfLargeButtons; i++)
	{
		Button& newLargeButton = buttons[i];
		newLargeButton.texture = tex;
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
		newSmallButton.texture = tex;
		newSmallButton.action = i;
		newSmallButton.renderTarget = Quad(glm::vec2(1 - buttonOffsetX_Small - buttonWidth_Small, 1 - buttonOffsetY_Small - buttonHeight_Small),
										   glm::vec2(1 - buttonOffsetX_Small, 1 - buttonOffsetY_Small));
		newSmallButton.renderTarget.SetOpenGL();
		if (i % 2 != 0)
		{
			buttonOffsetX_Small += (buttonWidth_Small + changeOffset_ButtonX_Small);
			buttonOffsetY_Small -= (buttonHeight_Small + changeOffset_ButtonY_Small);
		}
		else
			buttonOffsetY_Small += (buttonHeight_Small + changeOffset_ButtonY_Small);
	}

	while (!selectionMade)
	{
		menu_cam->GetComponent<Menu_Camera>().Update(0);
		glm::dmat4 camMatrix = menu_cam->GetComponent<Menu_Camera>().GetProjection() * menu_cam->GetComponent<Menu_Camera>().GetView();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0, 1, 0, 1);
		shader.Use();

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

		if (glfwWindowShouldClose(GameEngine::Get().GetWindow()))
		{
			selectionMade = true;
			return CLOSE;
		}

		glfwPollEvents();

	}
	return SelectionPicked();
}

void SettingsMenu::SelectionUp()
{
}

void SettingsMenu::SelectionDown()
{
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
