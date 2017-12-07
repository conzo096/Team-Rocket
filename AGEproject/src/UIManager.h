/* This class handles the in game UI elements.*/
#pragma once
#include "UIQuad.h"
#include <map>
class UIManager
{
	// What font the UI is using.
	unsigned int font;

	// List of all the UI elements.
	std::map<std::string, UIQuad> uiElements;
public: 

	UIManager();

	void Update(double deltaTime);

	void Render();



};