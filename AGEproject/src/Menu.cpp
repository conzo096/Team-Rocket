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
	unsigned int tex = GameEngine::Instance()->LoadTextures("../res/textures/debug.png");
	glm::mat4 m(1.0);
	glm::mat4 v(1.0);
	glm::mat4 p(1.0);
	glm::mat4 mvp(1.0);
	for (int i = 0; i < 3; i++)
	{
		Button& newButton = buttons[i];
		newButton.action = i;
		newButton.texture = tex; 
	}

	// Change while condition.
	while (!UserControls::get().IsKeyPressed("Enter"))
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1, 0, 0, 1);
		shader.Use();

		if (UserControls::get().IsKeyPressed("Forward"))
			SelectionUp();
		if (UserControls::get().IsKeyPressed("BackWard"))
			SelectionDown();
		// Bind texture.
		glUniform1i(shader.GetUniformLocation("tex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, buttons.at(0).texture);
		// Draw the quad.

		for (int i = 0; i < 3; i++)
		{		
			glUniformMatrix4fv(shader.GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
			buttons[i].renderTarget.Draw();
		}
		glfwSwapBuffers(GameEngine::Instance()->GetWindow());
		glfwPollEvents();
	}

	return SelectionPicked();
}
