#include "StateManager.h"
#include "Game.h"
#include "Entity.h"
#include "Renderable.h"
#include "UserControls.h"
#include "AudioEngine.h"
#include <memory>

void StateManager::StateLoop()
{
	GameEngine::Get().Initialise();
	bool running = true;
	int select;

	AudioEngine::Get().LoadSound("../res/audio/tone-beep.wav", false, false, false);
	AudioEngine::Get().PlaySound("../res/audio/tone-beep.wav");

	while (running)
	{
		glfwPollEvents();
		switch (state)
		{
		case(stateSplash):
			ShowSplashScreen();
			state = stateMainMenu;
			break;
		case(stateMainMenu):
			select = ShowMainMenu();
			if (select == 0)
			{
				Game::Get().Initialise(); //This will need a new home later.
				state = statePlaying;
			}
			else if (select == 1)
			{
				state = stateSettings;
			}
			else if (select == 2) { state = stateExiting; }
			break;
		case(stateSettings):
			select = ShowSettingsMenu();
			if (select == 1)
			{
				GameEngine::Get().UpdateWindow();
				state = stateMainMenu;
			}
			else if (select == 2) 
			{ 
				state = stateMainMenu; 
			}
			else if (select == 0)
			{
				state = stateControls;
			}
			else if (select == 7) 
			{ 
				state = stateExiting; 
			}
			break;
		case(stateControls):
			select = ShowControlsMenu();
			// Handle result.
			state = stateMainMenu;
			break;
		case(statePlaying):
			running = Game::Get().Update();
			Game::Get().Render();
			break;
		case(stateExiting):
			running = false;
			break;
		default:
			throw std::invalid_argument("Error: No behavior has been set for state" + state);
			break;
		}
	}
	AudioEngine::Get().UnloadSound("../res/audio/tone-beep.wav");
	GameEngine::Get().CleanUp();
}