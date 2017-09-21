#include "StateManager.h"

StateManager *StateManager::instance = 0;

void StateManager::StateLoop()
{
	//GameEngine::Instance()->Start();
	GameEngine::Instance()->Initialise();
	GameEngine::Instance()->ImSorryOrHowILearnedToStopCaringAndLoadTextures();

	while (!glfwWindowShouldClose(GameEngine::Instance()->GetWindow()))
	{
		switch (state)
		{
		case(Splash):
			GameEngine::Instance()->Render();
			break;
		case(Menu):
			break;
		case(Settings):
			break;
		case(Playing):
			break;
		case(Exiting):
			break;
		default:
			throw std::invalid_argument("Error: No behavior has been set for state" + state);
			break;
		}
	}
	GameEngine::Instance()->CleanUp();
}
