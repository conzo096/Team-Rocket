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

		UIQuad name;
		name.SetText("Name:");
		name.SetX(650);
		name.SetY(100);
		name.SetSize(10);
		uiElements.insert(std::pair<std::string, UIQuad>("Name", name));
	
		UIQuad health;
		health.SetText("Health:");
		health.SetX(650);
		health.SetY(75);
		health.SetSize(10);
		uiElements.insert(std::pair<std::string, UIQuad>("Health", health));
		UIQuad misc;
		misc.SetText("misc:");
		misc.SetX(650);
		misc.SetY(50);
		misc.SetSize(10);
		uiElements.insert(std::pair<std::string, UIQuad>("Misc", misc));
		UIQuad misc2;
		misc2.SetText("misc2:");
		misc2.SetX(650);
		misc2.SetY(25);
		misc2.SetSize(10);
		uiElements.insert(std::pair<std::string, UIQuad>("Misc2", misc2));

	}

	void UIManager::Update(double deltaTime)
	{
		// Update UI label.
		auto& x = uiElements.find("Balance");

		char  buffer[64];
		// Format and print various data:   
		sprintf_s(buffer, 64, "Bal:%i\n", Game::Get().GetPlayer().GetBalance());
		x->second.SetText(buffer);

		// If there is a selected unit, print its info.
		if (Game::Get().GetPlayer().GetSelectedEntities().size() > 0)
		{			// Only interested in first unit.
			x = uiElements.find("Name");
			x->second.SetIsActive(true);
			x->second.SetText(Game::Get().GetPlayer().GetSelectedEntities()[0]->GetName().c_str());
			x = uiElements.find("Health");
			x->second.SetIsActive(true);
			sprintf_s(buffer, 64, "%.0f\n", Game::Get().GetPlayer().GetSelectedEntities()[0]->GetComponent<Targetable>().GetHealth());
			x->second.SetText(buffer);
			x = uiElements.find("Misc");
			x->second.SetIsActive(true);
			// If the selected entity is a structure, print out number of queued items.
			if (Game::Get().GetPlayer().GetSelectedEntities()[0]->GetCompatibleComponent<Structure>() != NULL)
			{
				if (Game::Get().GetPlayer().GetSelectedEntities()[0]->GetCompatibleComponent<Structure>()->GetQueueSize() > 0)
					x->second.SetText(Game::Get().GetPlayer().GetSelectedEntities()[0]->GetCompatibleComponent<Structure>()->GetNextProduct().productName.c_str());
				else
				{
					x->second.SetText("None!");
				}
			}
			else if (Game::Get().GetPlayer().GetSelectedEntities()[0]->GetCompatibleComponent<Unit>() != NULL)
			{
				sprintf_s(buffer, 64, "%i\n", Game::Get().GetPlayer().GetSelectedEntities()[0]->GetCompatibleComponent<Unit>()->GetAction());
				x->second.SetText(buffer);
			}
			// If it is a worker, print out unit info.
			if (Game::Get().GetPlayer().GetSelectedEntities()[0]->GetName() == "Worker")
			{
				x = uiElements.find("Misc2");
				x->second.SetIsActive(true);
				sprintf_s(buffer, 64, "%i\n", Game::Get().GetPlayer().GetSelectedEntities()[0]->GetCompatibleComponent<Unit>()->GetAction());
				x->second.SetText(buffer);
			}
		}
		else
		{
			x = uiElements.find("Name");
			x->second.SetIsActive(false);
			x = uiElements.find("Health");
			x->second.SetIsActive(false);
			x = uiElements.find("Misc");
			x->second.SetIsActive(false);
			x = uiElements.find("Misc2");
			x->second.SetIsActive(false);
		}
	}

	void UIManager::Render()
	{
		for (auto& x : uiElements)
		{
			x.second.Render();
		}
	}