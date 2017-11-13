#include "MainMenu.h"
#include "RayCast.h"
#include "GeometryUtil.h"
#include "UserControls.h"

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
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);
		shader.Use();
		// If cursor is over a button, highlight it
		if (!mouseButtonHeld)
		{
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

		//if (UserControls::Get().IsMouseButtonPressed(std::string("Action")))
		//{
		//	if (!mouseButtonHeld)
		//	{
		//		// "Start Game" is clicked
		//		if (buttons[0].renderTarget.IsMouseInBounds())
		//		{
		//			currentSelection = 0;
		//		}
		//		// "Options" is clicked
		//		else if (buttons[1].renderTarget.IsMouseInBounds())
		//		{
		//			currentSelection = 1;
		//		}
		//		// "Exit Game" is clicked
		//		else if (buttons[2].renderTarget.IsMouseInBounds())
		//		{
		//			currentSelection = 2;
		//		}
		//		mouseButtonHeld = true;
		//	}
		//}
		//else
		//{
		//	if (mouseButtonHeld)
		//	{
		//		mouseButtonHeld = false;
		//		if (currentSelection > -1)
		//		{
		//			if (buttons[currentSelection].renderTarget.IsMouseInBounds())
		//				selectionMade = true;
		//			else
		//				currentSelection = -1;
		//		}
		//	}
		//}

		selectionMade = UserControls::Get().MouseSelection(std::string("Action"), buttons, mouseButtonHeld, currentSelection);

		if (glfwWindowShouldClose(GameEngine::Get().GetWindow()))
		{
			selectionMade = true;
			currentSelection = 2;
		}

		glfwPollEvents();
	}
	return currentSelection;
}