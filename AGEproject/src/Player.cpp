#include "Player.h"
#include "UserControls.h"
#include "BoundingBox.h"
void Player::Update()
{
	if (!isNPC)
	{
		HandleInput();
	}

}

void Player::HandleInput()
{
	// Handle selection action.
	if (UserControls::Get().IsMouseButtonPressed(std::string("Action")))
	{
		// Find if there is an intersection with any entity.
		for (Entity* &e : entities)
		{
			if(e->GetComponent<BoundingBox>)
		}
	}

}

void Player::Render()
{


}
