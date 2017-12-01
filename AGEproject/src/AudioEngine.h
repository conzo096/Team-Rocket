#pragma once
#include "FMOD\fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>
#include "glm\glm.hpp"

using namespace std;

struct Implementation
{
	Implementation();
	~Implementation();

	void Update();

	FMOD::System* mpSystem;

	int mnNextChannelID;

	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;

	SoundMap mSounds;
	ChannelMap mChannels;
};

