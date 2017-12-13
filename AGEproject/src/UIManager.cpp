#include "UIManager.h"
#include "Game.h"
#include "UserControls.h"
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

		// Friendly selected unit
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

		//Enemy selected unit.
		// Friendly selected unit
		
		name.SetText("Name:");
		name.SetX(0);
		name.SetY(100);
		name.SetSize(10);
		name.SetTextColour(glm::vec4(255, 0, 0, 1));
		uiElements.insert(std::pair<std::string, UIQuad>("EnemyName", name));

		health.SetText("Health:");
		health.SetX(0);
		health.SetY(75);
		health.SetSize(10);
		health.SetTextColour(glm::vec4(255, 0, 0, 1));
		uiElements.insert(std::pair<std::string, UIQuad>("EnemyHealth", health));
		
		misc.SetText("misc:");
		misc.SetX(0);
		misc.SetY(50);
		misc.SetSize(10);
		misc.SetTextColour(glm::vec4(255, 0, 0, 1));
		uiElements.insert(std::pair<std::string, UIQuad>("EnemyMisc", misc));
		
		misc2.SetText("misc2:");
		misc2.SetX(0);
		misc2.SetY(25);
		misc2.SetSize(10);
		misc2.SetTextColour(glm::vec4(255, 0, 0, 1));
		uiElements.insert(std::pair<std::string, UIQuad>("EnemyMisc2", misc2));


		name.SetText("HotKey1");
		name.SetX(200);
		name.SetY(25);
		name.SetSize(8);
		name.SetIsActive(true);
		name.SetTextColour(glm::vec4(255, 255, 255, 1));
		uiElements.insert(std::pair<std::string, UIQuad>("HotKey1", name));

		name.SetText("HotKey2");
		name.SetX(350);
		name.SetY(25);
		name.SetSize(8);
		name.SetIsActive(true);
		uiElements.insert(std::pair<std::string, UIQuad>("HotKey2", name));

		name.SetText("HotKey3");
		name.SetX(500);
		name.SetY(25);
		name.SetSize(8);
		name.SetIsActive(true);
		uiElements.insert(std::pair<std::string, UIQuad>("HotKey3", name));
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
		if (Game::Get().GetPlayer().GetSelectedFriendlyEntity() != NULL)
		{			// Only interested in first unit.
			x = uiElements.find("Name");
			x->second.SetIsActive(true);
			if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>() != NULL && Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Unit>() == NULL)
			{
				std::string text = Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetName();
				text += " V";
				text += std::to_string(Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetRank());
				x->second.SetText(text.c_str());
			}
			else
				x->second.SetText(Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetName().c_str());
			x = uiElements.find("Health");
			x->second.SetIsActive(true);
			sprintf_s(buffer, 64, "%.0f\n", Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetComponent<Targetable>().GetHealth());
			x->second.SetText(buffer);

			// Set the third info quad.
			x = uiElements.find("Misc");
			x->second.SetIsActive(true);
			// If the selected entity is a structure, print out number of queued items.
			if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>() != NULL)
			{
				// If there is an item in queue display name, else print none.
				if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetQueueSize() > 0)
					x->second.SetText(Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetNextProduct().productName.c_str());
				else
				{
					x->second.SetText("None!");
				}
			}
			// If it is just a combat unit print out there action.
			else if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Unit>() != NULL)
			{
				x->second.SetText(UnitActions[Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Unit>()->GetAction()].c_str());
			}
			// Workers are considered both - print their action out underneath the tag.
			if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetName() == "Worker")
			{
				x = uiElements.find("Misc2");
				x->second.SetIsActive(true);
				x->second.SetText(UnitActions[Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Unit>()->GetAction()].c_str());
			}


			// Handle hotkeys actions.

			// If it is  a structure - Handle their options.
			if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>() != NULL)
			{
				// Check how many spawn units it has.
				int num = Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetSpawnInfo().size();
			
				if (num > 0)
				{
					auto& y = uiElements.find("HotKey1");
					y->second.SetIsActive(true);
					// Hotkey names - Replace the number with the character value.
					std::string hk = "1:";
					hk += Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetSpawnInfo()[0].unitType;
					// Bracket the cost after.
					hk += "(" + std::to_string(Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetSpawnInfo()[0].cost) + ")";
					y->second.SetText(hk.c_str());
					if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetSpawnInfo()[0].cost > Game::Get().GetPlayer().GetBalance())
						y->second.SetTextColour(glm::vec4(255, 0, 0, 1));
					else
						y->second.SetTextColour(glm::vec4(0, 120, 0, 1));
					if (num > 1)
					{
						auto& y = uiElements.find("HotKey2");
						y->second.SetIsActive(true);
						// Hotkey names
						std::string hk = "2:";
						hk += Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetSpawnInfo()[1].unitType;
						// Bracket the cost after.
						hk += "(" + std::to_string(Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetSpawnInfo()[1].cost) + ")";
						y->second.SetText(hk.c_str());
						if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetSpawnInfo()[0].cost > Game::Get().GetPlayer().GetBalance())
							y->second.SetTextColour(glm::vec4(255, 0, 0, 1));
						else
							y->second.SetTextColour(glm::vec4(0, 120, 0, 1));
						if (num > 2)
						{
							auto& y = uiElements.find("HotKey3");
							y->second.SetIsActive(true);
							// Hotkey names
							std::string hk = "3:";
							hk += Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetSpawnInfo()[2].unitType;
							// Bracket the cost after.
							hk += "(" + std::to_string(Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetSpawnInfo()[2].cost) + ")";
							y->second.SetText(hk.c_str());
							if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetSpawnInfo()[0].cost > Game::Get().GetPlayer().GetBalance())
								y->second.SetTextColour(glm::vec4(255, 0, 0, 1));
							else
								y->second.SetTextColour(glm::vec4(0, 120, 0, 1));
						}
					}
				}
				
				
				// Non worker structures can buy an update. 
				if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetName() != "Worker")
				{
					// Second hotkey allows for an update.
					auto& y = uiElements.find("HotKey2");
					y->second.SetIsActive(true);
					// Hotkey names
					std::string hk = "2:";
					if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetRank() < 2)
					{
						hk += "Upgrade";
						// Bracket the cost after.
						hk += "(" + std::to_string(Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetUpdateCost()) + ")";
						if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>()->GetUpdateCost() > Game::Get().GetPlayer().GetBalance())
							y->second.SetTextColour(glm::vec4(255, 0, 0, 1));
						else
							y->second.SetTextColour(glm::vec4(0, 120, 0, 1));
					}
					else
					{
						hk += "MAX";
						y->second.SetTextColour(glm::vec4(255, 0, 0, 1));
					}
					y->second.SetText(hk.c_str());
				}
				
				// Now combat units.
				if (Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Structure>() == NULL && Game::Get().GetPlayer().GetSelectedFriendlyEntity()->GetCompatibleComponent<Unit>() != NULL)
				{
					// Second action can heal them?

				}
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
			x = uiElements.find("HotKey1");
			x->second.SetIsActive(false);
			x = uiElements.find("HotKey2");
			x->second.SetIsActive(false);
			x = uiElements.find("HotKey3");
			x->second.SetIsActive(false);
			//DeselectEnemyLabel();
		}
	}

	void UIManager::Render()
	{
		for (auto& x : uiElements)
		{
			x.second.Render();
		}
	}


	void UIManager::UpdateEnemyLabels(std::shared_ptr<Entity> enemy)
	{		
		if (enemy == NULL)
			DeselectEnemyLabel();
		else
		{
			char buffer[64];
			auto& x = uiElements.find("EnemyName");
			x->second.SetIsActive(true);
			x->second.SetTextColour(glm::vec4(255, 0, 0, 1));
			x->second.SetText(enemy->GetName().c_str());
			x = uiElements.find("EnemyHealth");
			x->second.SetIsActive(true);
			x->second.SetTextColour(glm::vec4(255, 0, 0, 1));
			if (enemy->GetCompatibleComponent<Targetable>()->GetHealth()!= NULL)
			{
				sprintf_s(buffer, 64, "%.0f\n", enemy->GetComponent<Targetable>().GetHealth());
				x->second.SetText(buffer);
			}
			else
				x->second.SetText("0");
			// Set the third info quad.
			x = uiElements.find("EnemyMisc");
			x->second.SetIsActive(true);
			x->second.SetTextColour(glm::vec4(255, 0, 0, 1));
			x->second.SetIsActive(true);
			// If the selected entity is a structure, print out number of queued items.
			if (enemy->GetCompatibleComponent<Structure>() != NULL)
			{
				// If there is an item in queue display name, else print none.
				if (enemy->GetCompatibleComponent<Structure>()->GetQueueSize() > 0)
					x->second.SetText(enemy->GetCompatibleComponent<Structure>()->GetNextProduct().productName.c_str());
				else
				{
					x->second.SetText("None!");
				}
			}
			// If it is just a combat unit print out there action.
			else if (enemy->GetCompatibleComponent<Unit>() != NULL)
			{
				x->second.SetIsActive(true);
				x->second.SetTextColour(glm::vec4(255, 0, 0, 1));
				x->second.SetText(UnitActions[enemy->GetCompatibleComponent<Unit>()->GetAction()].c_str());
			}
			// Workers are considered both - print their action out underneath the tag.
			if (enemy->GetName() == "Worker")
			{
				x = uiElements.find("EnemyMisc2");
				x->second.SetIsActive(true);
				x->second.SetTextColour(glm::vec4(255, 0, 0, 1));
				x->second.SetText(UnitActions[enemy->GetCompatibleComponent<Unit>()->GetAction()].c_str());
			}
		}
	}

	void UIManager::DeselectEnemyLabel()
	{
		auto& x = uiElements.find("EnemyName");
		x->second.SetIsActive(false);
		x = uiElements.find("EnemyHealth");
		x->second.SetIsActive(false);
		x = uiElements.find("EnemyMisc");
		x->second.SetIsActive(false);
		x = uiElements.find("EnemyMisc2");
		x->second.SetIsActive(false);
	}