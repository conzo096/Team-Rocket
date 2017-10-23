#include "Menu.h"
#include "RayCast.h"
#include "GeometryUtil.h"

Menu::Menu(std::vector<char *> textureLocs) {}


void Menu::SelectionUp()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the first one, loop to end one. 
	if (currentSelection == 0)
		currentSelection = buttons.size()-1;
	currentSelection -= 1;
}

void Menu::SelectionDown()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the last one one, loop to first one. 
	if (currentSelection == buttons.size() - 1)
		currentSelection = 0;
	currentSelection += 1;
}

int Menu::SelectionPicked()
{
	return buttons.at(currentSelection).action;
}

int Menu::Draw(GLShader shader)
{
	int numberOfButtons = 3;
	buttons.resize(numberOfButtons);
	unsigned int tex = Texture("../res/textures/debug.png").GetTextureId();
	// 3 buttons have to fit in a screen space of two (-1 to 1).

	// % size of the button (1 = 100%, 0.5 - 50% etc)
	float buttonWidth = 0.6f;
	float buttonHeight = 0.3f;
	float buttonOffset = 0.5f;
	const float offsetChange = 0.5f;

	// Starting position for the button is the top - the button height.
	for (int i = 0; i < numberOfButtons; i++)
	{
		Button& newButton = buttons[i];
		newButton.action = i;
		newButton.texture = tex; 
		newButton.renderTarget = Quad(glm::vec2(0 - (buttonWidth / 2.0f), (1-i-buttonOffset) - (buttonHeight / 2.0f)),
									  glm::vec2(0 + (buttonWidth / 2.0f), (1-i-buttonOffset) + (buttonHeight / 2.0f)));
		newButton.renderTarget.SetOpenGL();
		buttonOffset -= offsetChange;
	}

	// Change while condition.
	while (!UserControls::Get().IsKeyPressed(std::string("Enter")))
	{
		menu_cam->GetComponent<Menu_Camera>().Update(0);
		glm::dmat4 camMatrix = menu_cam->GetComponent<Menu_Camera>().GetProjection() * menu_cam->GetComponent<Menu_Camera>().GetView();
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 1, 0, 1);
		shader.Use();

		if (UserControls::Get().IsKeyPressed(std::string("Forward")))
			SelectionUp();
		if (UserControls::Get().IsKeyPressed(std::string("Backward")))
			SelectionDown();

		if (UserControls::Get().IsMouseButtonPressed(std::string("Action")))
		{
		
		}

		// Draw the quad.
  		for (int i = 0; i < 3; i++)
		{		
			if (glfwGetMouseButton(GameEngine::Get().GetWindow(), 0))
			{
				if (buttons[i].renderTarget.IsMouseInBounds())
					std::cout << i << std::endl;
			}
			// Bind texture.
			glUniform1i(shader.GetUniformLocation("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, buttons.at(i).texture);
			buttons[i].renderTarget.Draw();
		}
		glfwSwapBuffers(GameEngine::Get().GetWindow());
		glfwPollEvents();
	}

	return SelectionPicked();
}
