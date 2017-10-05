#include "Menu.h"


TMenu::TMenu(std::vector<char *> textureLocs)
{

	for (int i = 0; i < textureLocs.size(); i++)
	{
		Button newButton;
		newButton.action = i;
		newButton.texture = Texture(textureLocs.at(i),500,300);
		newButton.renderTarget.SetPosition(glm::vec3(0, 0, i / textureLocs.size()));
		newButton.renderTarget.UpdateTransforms();
	}

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
	for (int i = 0; i < 3; i++)
	{
		Button newButton;
		newButton.action = i;
		newButton.texture = Texture("../res/textures/debug.png", 500, 300);
		newButton.renderTarget.SetPosition(glm::vec3(0, 0, i / 3));
		newButton.renderTarget.UpdateTransforms();
	}
	shader.Use();
	// Change while condition.
	while (true)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (UserControls::get().IsKeyPressed("Forward"))
			SelectionUp();
		if (UserControls::get().IsKeyPressed("BackWard"));
			SelectionDown();
		if (UserControls::get().IsKeyPressed("Enter"));
			return SelectionPicked();
	
		for (int i = 0; i < buttons.size(); i++)
		{
			// Bind texture.
			glUniform1i(shader.GetUniformLocation("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, buttons.at(i).texture.texture);
			// Set the MVP.
			glm::mat4 t = buttons.at(i).renderTarget.GetTransform();
			glUniformMatrix4fv(shader.GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(t));
			// Draw the quad.
			buttons.at(i).renderTarget.Draw();
		}

	}
}
