#pragma once
#include "Menu.h"
#include "UIQuad.h"

class ControlsMenu : public Menu
{
public:
	ControlsMenu() 
	{
		button_tex.push_back(ResourceHandler::Get().GetTexture("Forward"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Backward"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Left"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Right"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("RotateLeft"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("RotateRight"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Reset"));

		button_tex.push_back(ResourceHandler::Get().GetTexture("ZoomIn"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("ZoomOut"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Hold"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey1"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey2"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("HotKey3"));
		button_tex.push_back(ResourceHandler::Get().GetTexture("Back"));

		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Forward_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Backward_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Left_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Right_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("RotateLeft_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("RotateRight_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Reset_HIGHLIGHTED"));

		highlight_tex.push_back(ResourceHandler::Get().GetTexture("ZoomIn_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("ZoomOut_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Hold_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("HotKey1_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("HotKey2_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("HotKey3_HIGHLIGHTED"));
		highlight_tex.push_back(ResourceHandler::Get().GetTexture("Back_HIGHLIGHTED"));

		current_tex = button_tex;


		if (buttons.size() == 0)
		{
			// Initialise the quads.
			buttons.resize(numOfControls);
			// Set up binding vector.
			PopulateBindings();
			float buttonWidth = 0.46875f * 0.7f;
			float buttonHeight = 0.20926f * 0.7f;
			float resetWidth = 0.36458f * 0.7f;
			float resetHeight = 0.27778f * 0.7f;

			float buttonOffsetX = 0.2f;
			float buttonOffsetY = 0.1f;
			float offsetChange = 0.1f + buttonHeight;

			// Handle left hand side.
			for (int i = 0; i < (buttons.size() / 2); i++)
			{
				Button& newButton = buttons[i].first;
				newButton.action = i;
				newButton.texture = current_tex[i];

				if (!(i == 6))
				{
					newButton.renderTarget = Quad(glm::vec2(-1 + buttonOffsetX, 1 - buttonOffsetY - buttonHeight),
						glm::vec2(-1 + buttonOffsetX + buttonWidth, 1 - buttonOffsetY));

					// Draw text.
					UIQuad& u = buttons[i].second;
					u.SetText(bindings[i].c_str());
					u.SetSize(12);
					u.SetX(((buttonOffsetX + buttonWidth + 0.05f) / 2) * 800);
					u.SetY(((2 - buttonOffsetY - (buttonHeight * 0.57f)) / 2) * 600);

					buttonOffsetY += offsetChange;
				}
				else
				{
					buttonOffsetX = 0.05f;
					buttonOffsetY += 0.1f;
					newButton.renderTarget = Quad(glm::vec2(0 - buttonOffsetX - resetWidth, 1 - buttonOffsetY - resetHeight),
						glm::vec2(0 - buttonOffsetX, 1 - buttonOffsetY));
				}

				newButton.renderTarget.SetOpenGL();
			}

			// Handle right hand side.
			buttonOffsetX = 0.1f;
			buttonOffsetY = 0.1f;
			for (int i = (buttons.size() / 2); i < buttons.size(); i++)
			{
				Button& newButton = buttons[i].first;
				newButton.action = i;
				newButton.texture = current_tex[i];

				if (!(i == 13))
				{
					newButton.renderTarget = Quad(glm::vec2(0 + buttonOffsetX, 1 - buttonOffsetY - buttonHeight),
						glm::vec2(0 + buttonOffsetX + buttonWidth, 1 - buttonOffsetY));

					// Draw text.
					UIQuad& u = buttons[i].second;
					u.SetText(bindings[i].c_str());
					u.SetSize(12);
					u.SetX(((1 + buttonOffsetX + buttonWidth + 0.05f) / 2) * 800);
					u.SetY(((2 - buttonOffsetY - (buttonHeight * 0.57f)) / 2) * 600);

					buttonOffsetY += offsetChange;
				}
				else
				{
					buttonOffsetX = 0.05f;
					buttonOffsetY += 0.1f;
					newButton.renderTarget = Quad(glm::vec2(0 + buttonOffsetX, 1 - buttonOffsetY - resetHeight),
						glm::vec2(0 + buttonOffsetX + resetWidth, 1 - buttonOffsetY));
				}

				newButton.renderTarget.SetOpenGL();
			}

		}

	}
	~ControlsMenu() {}

	int Draw(GLShader shader);

	void SelectionUp();
	void SelectionDown();
	void SelectionLeft();
	void SelectionRight();
	int SelectionPicked();

private:
	static int currentSelection;
	static int lastSelection;
	static const int numOfControls;

	// Buttons which display their key binding.
	static std::vector<std::pair<Button, UIQuad>> buttons;

	// Button textures
	std::vector <unsigned int> button_tex;
	std::vector <unsigned int> highlight_tex;
	std::vector <unsigned int> current_tex;

	// Key bindings, index of buttons represents one of these.
	static std::vector<std::string> bindings;
	// What each quad represents.
	static std::vector<std::string> buttonActions;
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void PopulateBindings();
};