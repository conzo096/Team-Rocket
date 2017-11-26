#include "UIManager.h"

	// Default constructor for UI.
	UIManager::UIManager()
	{
		// Add balance.
		UIQuad balance;
		//balance.SetText("HELP");

		uiElements.insert(std::pair<std::string,UIQuad>("Balance", balance));
	}

	void UIManager::Update(double deltaTime)
	{
	
	
	}

	void UIManager::Render()
	{
		for (auto& x : uiElements)
		{
			x.second.Render();
		}
	}