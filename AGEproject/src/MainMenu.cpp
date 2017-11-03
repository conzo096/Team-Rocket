#include "MainMenu.h"
#include "RayCast.h"
#include "GeometryUtil.h"

// Move up
void MainMenu::SelectionUp()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the first one, loop to end one. 
	if (currentSelection == 0)
		currentSelection = buttons.size() - 1;
	currentSelection -= 1;
}

// Move down.
void MainMenu::SelectionDown()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the last one one, loop to first one. 
	if (currentSelection == buttons.size() - 1)
		currentSelection = 0;
	currentSelection += 1;
}

// Return selected button action.
int MainMenu::SelectionPicked()
{
	return buttons.at(currentSelection).action;
}

int MainMenu::Draw(GLShader shader)
{
	buttons.resize(numberOfButtons);
	unsigned int normal_tex[3];
	unsigned int highlight_tex[3];

	normal_tex[0] = Texture("../res/textures/MainMenu_Button1.png").GetTextureId();
	normal_tex[1] = Texture("../res/textures/MainMenu_Button2.png").GetTextureId();
	normal_tex[2] = Texture("../res/textures/MainMenu_Button3.png").GetTextureId();

	highlight_tex[0] = Texture("../res/textures/MainMenu_Button1_Highlighted.png").GetTextureId();
	highlight_tex[1] = Texture("../res/textures/MainMenu_Button2_Highlighted.png").GetTextureId();
	highlight_tex[2] = Texture("../res/textures/MainMenu_Button3_Highlighted.png").GetTextureId();

	// 3 buttons have to fit in a screen space of two (-1 to 1).

	for (int i = 0; i < numberOfButtons; i++)
	{
		Button& newButton = buttons[i];

		newButton.action = i;
		newButton.texture = normal_tex[i];
		newButton.renderTarget = Quad(glm::vec2(0 - (buttonWidth / 2.0f), 1 - buttonOffset - buttonHeight),
									  glm::vec2(0 + (buttonWidth / 2.0f), 1 - buttonOffset));
		newButton.renderTarget.SetOpenGL();
		buttonOffset += offsetChange;
	}

	while (!selectionMade)
	{
		menu_cam->GetComponent<Menu_Camera>().Update(0);
		glm::dmat4 camMatrix = menu_cam->GetComponent<Menu_Camera>().GetProjection() * menu_cam->GetComponent<Menu_Camera>().GetView();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);
		shader.Use();
		// If cursor is over a button, highlight it
		for (int i = 0; i < numberOfButtons; i++)
		{
			if (buttons[i].renderTarget.IsMouseInBounds())
			{
				buttons[i].texture = highlight_tex[i];
				break;
			}
			else
				buttons[i].texture = normal_tex[i];
		}

		// Draw the quad.
		for (int i = 0; i < numberOfButtons; i++)
		{
			// Bind texture.
			glUniform1i(shader.GetUniformLocation("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, buttons.at(i).texture);
			buttons[i].renderTarget.Draw();
		}
		glfwSwapBuffers(GameEngine::Get().GetWindow());

		if (UserControls::Get().IsKeyPressed(std::string("Forward")))
			SelectionUp();
		if (UserControls::Get().IsKeyPressed(std::string("Backward")))
			SelectionDown();

		if (UserControls::Get().IsMouseButtonPressed(std::string("Action")))
		{
			// "Start Game" is clicked
			if (buttons[0].renderTarget.IsMouseInBounds())
			{
				selectionMade = true;
				currentSelection = 0;
			}
			// "Options" is clicked
			else if (buttons[1].renderTarget.IsMouseInBounds())
			{
				selectionMade = true;
				currentSelection = 1;
			}
			// "Exit Game" is clicked
			else if (buttons[2].renderTarget.IsMouseInBounds())
			{
				selectionMade = true;
				currentSelection = 2;
			}
		}

		if (glfwWindowShouldClose(GameEngine::Get().GetWindow()))
		{
			selectionMade = true;
			currentSelection = 2;
		}

		glfwPollEvents();
	}
	return SelectionPicked();
}