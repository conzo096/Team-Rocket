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
	
	// Render titles
	for (int i = 0; i < numOfTitles; i++)
	{
		Label& newTitle = labels[i];
		newTitle.texture = tex;
		newTitle.renderTarget = Quad(glm::vec2(0 - (titleWidth / 2.0f), 0 - (titleHeight / 2.0f)),
									 glm::vec2(0 + (titleWidth / 2.0f), 0 + (titleHeight / 2.0f)));
		newTitle.renderTarget.SetOpenGL();
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
		//for (int i = 0; i < numOfButtons; i++)
		//{
		//	// Bind texture.
		//	glUniform1i(shader.GetUniformLocation("tex"), 0);
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, buttons.at(i).texture);
		//	buttons[i].renderTarget.Draw();
		//}
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
