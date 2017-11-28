#include "ControlsMenu.h"
#include "UserControls.h"

int ControlsMenu::Draw(GLShader shader)
{	
	// Initialise the quads.
	buttons.resize(16);
	float buttonOffset = 0.0625f;
	float offsetChange = 0.17 + 0.0625f;
	// Handle right hand side.
	for (int i = 0; i < 8; i++)
	{
		Button& newButton = buttons[i].first;
		newButton.action = i;
		newButton.texture = ResourceHandler::Get().GetTexture("debug");
		newButton.renderTarget = Quad(glm::vec2(-0.8, 1 - buttonOffset - 0.1),
			glm::vec2(-0.6, 1 - buttonOffset));
		newButton.renderTarget.SetOpenGL();
		buttonOffset += offsetChange;
	
		// Draw text.
		UIQuad& u = buttons[i].second;
		char  buffer[4];
		// Format and print various data:   
		sprintf_s(buffer, 4,"%i\n", i);
		u.SetText(buffer);
		u.SetSize(8);
		u.SetX(200);
		u.SetY(i * 70);
	}
	// Handle left hand side.
	buttonOffset = 0.0625f;
	for (int i = 8; i < 16; i++)
	{
		auto &p = buttons.at(i);
		Button& newButton = buttons[i].first;
		newButton.action = i;
		newButton.texture = ResourceHandler::Get().GetTexture("debug");
		newButton.renderTarget = Quad(glm::vec2(0.2, 1 - buttonOffset - 0.1),
			glm::vec2(0.4, 1 - buttonOffset));
		newButton.renderTarget.SetOpenGL();
		buttonOffset += offsetChange;
	}
	while (!selectionMade)
	{
		// Handle input.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);
		// Draw their corresponding bindings.
		for (int i = 0; i < 8; i++)
		{
			buttons[i].second.Render();
		}
		shader.Use();
		// Draw the quads.
		for (int i = 0; i < buttons.size(); i++)
		{
			// Bind texture.
			glUniform1i(shader.GetUniformLocation("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, buttons[i].first.texture);
			buttons[i].first.renderTarget.Draw();
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
