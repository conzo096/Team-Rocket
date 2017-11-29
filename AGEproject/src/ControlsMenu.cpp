#include "ControlsMenu.h"
#include "UserControls.h"

int ControlsMenu:: currentSelection = 0;
std::vector <std::pair<Button, UIQuad>> ControlsMenu::buttons;
std::vector <std::string> ControlsMenu::bindings;
// Key callback method.
void ControlsMenu::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto keyName = glfwGetKeyName(key, scancode);
	if (keyName != NULL)
	{
		std::cout << keyName << std::endl;
		buttons[currentSelection].second.SetText(keyName);
		UserControls::Get().BindKey(bindings[currentSelection], key);
		FileIO::Get().SaveIniFile();
	}
	else
	{
		std::cout << "Need to manually type key pressed!" << std::endl;
		buttons[currentSelection].second.SetText("Invalid");
	}

	// Remove key callback.
	glfwSetKeyCallback(GameEngine::Get().GetWindow(), NULL);
}


int ControlsMenu::Draw(GLShader shader)
{	
	// Set up binding vector.

	PopulateBindings();
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
		u.SetText(bindings[i].c_str());
		u.SetSize(12);
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


		// Draw text.
		UIQuad& u = buttons[i].second;
		u.SetText("temp");
		u.SetSize(12);
		u.SetX(600);
		u.SetY((i%8) * 70);

	}
	// Exit action.
	exit.action = 101;
	// Set its texture
	exit.texture = ResourceHandler::Get().GetTexture("debug");
	exit.renderTarget = Quad(glm::vec2(-0.1, -0.8), glm::vec2(0.1, -0.6));
	exit.renderTarget.SetOpenGL();


	// Render buttons.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0, 0, 1, 1);
	shader.Use();
	// Draw the quads.
	for (int i = 0; i < 16; i++)
	{
		// Bind texture.
		glUniform1i(shader.GetUniformLocation("tex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, buttons[i].first.texture);
			buttons[i].first.renderTarget.Draw();
	}
	glUniform1i(shader.GetUniformLocation("tex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, exit.texture);
	exit.renderTarget.Draw();
	// Draw their corresponding bindings.
	for (int i = 0; i < 16; i++)
	{
		buttons[i].second.Render();
	}

	glfwSwapBuffers(GameEngine::Get().GetWindow());

	while (!selectionMade)
	{
		// Handle input.

		// Update textures.
		for (int i = 0; i < 16; i++)
		{
			if (buttons[i].first.renderTarget.IsMouseInBounds())
			{
				buttons[i].first.texture = ResourceHandler::Get().GetTexture("Rock");
			}
			else
				buttons[i].first.texture = ResourceHandler::Get().GetTexture("debug");

		}
		// Handle selection.
		if (UserControls::Get().IsMouseButtonPressed(std::string("Action")))
		{
			for (int i = 0; i < 8; i++)
				if (buttons[i].first.renderTarget.IsMouseInBounds())
				{
					currentSelection = i;
				//	selectionMade = true;
					glfwSetKeyCallback(GameEngine::Get().GetWindow(), keyCallback);
				}
			// Check exit button.
			if (exit.renderTarget.IsMouseInBounds())
			{
				currentSelection = exit.action;
				selectionMade = true;
			}
		}
		// Render buttons.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);
		shader.Use();
		// Draw the quads.
		for (int i = 0; i < 16; i++)
		{
			// Bind texture.
			glUniform1i(shader.GetUniformLocation("tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, buttons[i].first.texture);
			buttons[i].first.renderTarget.Draw();
		}
		glUniform1i(shader.GetUniformLocation("tex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, exit.texture);
		exit.renderTarget.Draw();

		// Draw their corresponding bindings.
		for (int i = 0; i < 16; i++)
		{
			buttons[i].second.Render();
		}
		glfwPollEvents();
		glfwSwapBuffers(GameEngine::Get().GetWindow());
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


void ControlsMenu::PopulateBindings()
{
	bindings.resize(16);
	// Iterate through map.
	
	// Camera movement.
	bindings[0] = "Forward";
	bindings[1] = "Backward";
	bindings[2] = "Left";
	bindings[3] = "Right";
	bindings[4] = "RotateLeft";
	bindings[5] = "RotateRight";
	bindings[6] = "ResetCamera";
	bindings[7] = "Enter";

	// Hotkey/ entity options. 
	bindings[8] = "Hold";
	bindings[9] = "HotKey1";
	bindings[10] = "HotKey2";
	bindings[11] = "HotKey3";

}