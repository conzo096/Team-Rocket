#include "Menu.h"

TMenu::TMenu(std::vector<char *> textureLocs)
{

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

	// 3 buttons have to fit in a screen space of two (-1 to 1).
	float length = 3 / 2;
	// % size of the button (1 = 100%, 0.5 - 50% etc)
	float buttonWidth = 0.6;
	float buttonHeight = 0.3;

	// Starting position for the button is the top - the button height.
	float startPos = 1 - buttonHeight;
	//for (int i = 0; i < 3; i++)
	//{
	//	Button& newButton = buttons[i];
	//	newButton.action = i;
	//	newButton.texture = tex; 

	//	newButton.renderTarget = Quad(glm::vec3(0,(startPos -) , 0),buttonWidth,buttonHeight);
	//	newButton.renderTarget.OpenGL();
	////	newButton.renderTarget.SetPosition(glm::vec3(0, (i*-60) + 57, 0));
	////	newButton.renderTarget.SetScale(glm::vec3(20, 20, 20));
	////	buttons[i].renderTarget.UpdateTransforms();
	//}


	
	buttons[0].action = 0;
	buttons[0].texture = tex;
	buttons[0].renderTarget = Quad(glm::vec3(0, (0.8), 0), buttonWidth, buttonHeight);
	buttons[0].renderTarget.OpenGL();

	
	buttons[1].action = 1;
	buttons[1].texture = tex;
	buttons[1].renderTarget = Quad(glm::vec3(0, (0), 0), buttonWidth, buttonHeight);
	buttons[1].renderTarget.OpenGL();
	
	buttons[2].action = 2;
	buttons[2].texture = tex;
	buttons[2].renderTarget = Quad(glm::vec3(0, (-0.8), 0), buttonWidth, buttonHeight);
	buttons[2].renderTarget.OpenGL();


	// Change while condition.
	while (!UserControls::get().IsKeyPressed(std::string("Enter")))
	{
		menu_cam->GetComponent<Menu_Camera>().Update(0);
		glm::dmat4 camMatrix = menu_cam->GetComponent<Menu_Camera>().GetProjection() * menu_cam->GetComponent<Menu_Camera>().GetView();
		
	//	buttons[0].renderTarget.Rotate(glm::vec3(1,1,1));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 1, 0, 1);
		shader.Use();

		if (UserControls::get().IsKeyPressed(std::string("Forward")))
			SelectionUp();
		if (UserControls::get().IsKeyPressed(std::string("Backward")))
			SelectionDown();



  		for (int i = 0; i < 3; i++)
		{		
			if (glfwGetMouseButton(GameEngine::Instance()->GetWindow(), 0))
			{
				if (buttons[i].renderTarget.IsMouseInBounds())
					std::cout << i << std::endl;
			}
			// Bind texture.
			glUniform1i(shader.GetUniformLocation("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, buttons.at(i).texture);
		//	glm::mat4 mvp = camMatrix * buttons[i].renderTarget.GetTransform();
		//	glUniformMatrix4fv(shader.GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
			buttons[i].renderTarget.Draw();
		}
		glfwSwapBuffers(GameEngine::Instance()->GetWindow());
		glfwPollEvents();
	}

	return SelectionPicked();
}
