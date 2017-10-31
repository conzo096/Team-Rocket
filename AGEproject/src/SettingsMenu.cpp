#include "SettingsMenu.h"
#include "RayCast.h"
#include "GeometryUtil.h"

int SettingsMenu::Draw(GLShader shader)
{
	const int numOfLabels = numOfTitles + numOfOptions;
	const int numOfButtons = numOfSmallButtons + numOfLargeButtons;
	labels.resize(numOfLabels);
	buttons.resize(numOfButtons);
	
	// Render titles
	for (int i = 0; i < numOfTitles; i++)
	{
		Label& newTitle = labels[i];
		newTitle.texture = tex;
//		newTitle.renderTarget = Quad(glm::vec2(-1 + titleOffsetX, -1 + titleOffsetY), 
//		newTitle.renderTarget.SetOpenGL();

	}
}

void SettingsMenu::SelectionUp()
{
}

void SettingsMenu::SelectionDown()
{
}

void SettingsMenu::SelectionLeft()
{
}

void SettingsMenu::SelectionRight()
{
}

int SettingsMenu::SelectionPicked()
{
	return 0;
}
