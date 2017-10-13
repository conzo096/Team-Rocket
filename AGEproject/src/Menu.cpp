#include "Menu.h"

TMenu::TMenu(std::vector<char *> textureLocs)
{

	//for (int i = 0; i < textureLocs.size(); i++)
	//{
	//	Button newButton;
	//	newButton.action = i;
	//	newButton.texture = Texture(textureLocs.at(i),500,300);
	//	newButton.renderTarget.SetPosition(glm::vec3(0, 0, i / textureLocs.size()));
	//	newButton.renderTarget.UpdateTransforms();

	//}
}


void TMenu::SelectionUp()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the first one, loop to end one. 
	if (currentSelection == 0)
		currentSelection = buttons.size()-1;
	currentSelection -= 1;
}

void TMenu::SelectionDown()
{
	if (buttons.size() == 0)
		return;
	// If currentSelection is the last one one, loop to first one. 
	if (currentSelection == buttons.size() - 1)
		currentSelection = 0;
	currentSelection += 1;
}

int TMenu::SelectionPicked()
{
	return buttons.at(currentSelection).action;
}

int TMenu::Draw(GLShader shader)
{
	buttons.resize(3);
	unsigned int tex = Texture("../res/textures/debug.png").GetTextureId();
	

	for (int i = 0; i < 3; i++)
	{
		Button& newButton = buttons[i];
		newButton.action = i;
		newButton.texture = tex; 
		newButton.renderTarget.SetPosition(glm::vec3(0, (i*-60) + 57, 0));
		newButton.renderTarget.SetScale(glm::vec3(20, 20, 20));
		buttons[i].renderTarget.UpdateTransforms();
	}

	// Change while condition.
	while (!UserControls::Get().IsKeyPressed(std::string("Enter")))
	{
		menu_cam->GetComponent<Menu_Camera>().Update(0);
		glm::dmat4 camMatrix = menu_cam->GetComponent<Menu_Camera>().GetProjection() * menu_cam->GetComponent<Menu_Camera>().GetView();
		
		buttons[0].renderTarget.Rotate(glm::vec3(1,1,1));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 1, 0, 1);
		shader.Use();

		if (UserControls::Get().IsKeyPressed(std::string("Forward")))
			SelectionUp();
		if (UserControls::Get().IsKeyPressed(std::string("Backward")))
			SelectionDown();
		// Bind texture.
		glUniform1i(shader.GetUniformLocation("tex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, buttons.at(0).texture);
		// Draw the quad.

		for (int i = 0; i < 3; i++)
		{		
			glm::mat4 mvp = camMatrix * buttons[i].renderTarget.GetTransform();
			glUniformMatrix4fv(shader.GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
			buttons[i].renderTarget.Draw();
		}
		glfwSwapBuffers(GameEngine::Get().GetWindow());
		glfwPollEvents();
	}

	return SelectionPicked();
}
