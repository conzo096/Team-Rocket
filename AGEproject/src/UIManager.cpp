#include "UIManager.h"
#include "Game.h"
	// Default constructor for UI.
	UIManager::UIManager()
	{
		// Add balance.
		UIQuad balance;
		balance.SetText("Bal:");
		balance.SetX(0);
		balance.SetY(550);
		balance.SetSize(10);
		uiElements.insert(std::pair<std::string,UIQuad>("Balance", balance));

	}

	void UIManager::Update(double deltaTime)
	{
		// Update UI label.
		auto& x = uiElements.find("Balance");

		char  buffer[64];
		// Format and print various data:   
		sprintf_s(buffer, 64, "Bal:%i\n", Game::Get().GetPlayer().GetBalance());
		x->second.SetText(buffer);
	}

	void UIManager::Render()
	{
		for (auto& x : uiElements)
		{
			x.second.Render();
		}
	}