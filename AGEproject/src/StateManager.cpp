#include "StateManager.h"

StateManager *StateManager::instance = 0;

void StateManager::StateLoop()
{
	GameEngine::Instance()->Start();

	while (!glfwWindowShouldClose(GameEngine::Instance()->GetWindow()))
	{
		switch (state)
		{
		case(Splash):
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
}
