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
	currentSelection -= 1;
	if (currentSelection < 0)
		currentSelection = static_cast<unsigned int>(buttons.size()) - 1;
}

// Move down.
void MainMenu::SelectionDown()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the last one one, loop to first one. 
	currentSelection += 1;
	if (currentSelection > buttons.size() - 1)
		currentSelection = 0;
}

// Return selected button action.
int MainMenu::SelectionPicked()
{
	return buttons.at(currentSelection).action;
}

int MainMenu::Draw(GLShader shader)
{
	// Draw background texture
	Quad background = Quad();
	const unsigned int background_tex = ResourceHandler::Get().GetTexture("Background");
	background.SetOpenGL();

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

	// If a controller is being used...
	UserControls::Get().FindConnectedJoystick();
	if (UserControls::Get().isJoystickActive() == GL_TRUE)
		currentSelection = 0;

	while (!selectionMade)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);
		shader.Use();

		if (UserControls::Get().isJoystickActive())
		{
			timeElapsed++;
			if (timeElapsed > cooldown)
			{
				timeElapsed = 0;
				// process our joystick info
				if (UserControls::Get().IsJoystickPressed("dUp",UserControls::ControllerAction::BUTTON))
				{
					SelectionUp();
				}
				if (UserControls::Get().IsJoystickPressed("dDown", UserControls::ControllerAction::BUTTON))
				{
					SelectionDown();
				}
				for (int i = 0; i < buttons.size(); i++)
					if (i == currentSelection)
						buttons[i].texture = highlight_tex[i];
					else
						buttons[i].texture = normal_tex[i];
				if (UserControls::Get().IsJoystickPressed("A", UserControls::ControllerAction::BUTTON))
					break;
			}
		}
		else
		{
			// If cursor is over a button, highlight it
			if (!mouseButtonHeld)
			{
				for (int i = 0; i < numberOfButtons; i++)
				{
					if (buttons[i].renderTarget.IsMouseInBounds())
					{
						buttons[i].texture = highlight_tex[i];
					}
					else
						buttons[i].texture = normal_tex[i];
				}
			}
			if (UserControls::Get().IsKeyPressed(std::string("Forward")))
				SelectionUp();
			if (UserControls::Get().IsKeyPressed(std::string("Backward")))
				SelectionDown();
			selectionMade = UserControls::Get().MouseSelection(std::string("Action"), buttons, mouseButtonHeld, currentSelection);
		}
		if (glfwWindowShouldClose(GameEngine::Get().GetWindow()))
		{
			selectionMade = true;
			currentSelection = 2;
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

		glBindTexture(GL_TEXTURE_2D, background_tex);
		background.Draw();

		glfwPollEvents();
		glfwSwapBuffers(GameEngine::Get().GetWindow());
	}
	return currentSelection;
}