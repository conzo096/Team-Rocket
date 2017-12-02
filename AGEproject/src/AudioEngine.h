#pragma once
#include "FMOD\fmod.hpp"
#include "FMOD\fmod_studio.hpp"
#include <string>
#include <map>
#include <math.h>
#include "glm\glm.hpp"
#include "Singleton.h"

using namespace std;

struct Implementation
{
	Implementation();
	~Implementation();

	void Update();

	FMOD::System* audioSystem;

	int nextChannelID = 1;

	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;

	// The map of loaded sounds
	SoundMap mSounds;
	// The map of audio channels currently in use
	ChannelMap mChannels;
};

class AudioEngine : public Singleton<AudioEngine>
{
public:
	// Initialise FMOD
	void Initialise();
	// Check for unused audio channels and close them
	void Update();
	// Clean up FMOD
	void Shutdown();
	// Check for FMOD errors
	int ErrorCheck(FMOD_RESULT result);

	// Load a sound file: define if it is 3D, being read or streamed, and/or playing on a loop
	void LoadSound(const string& soundName, bool is3d = true, bool looping = false, bool stream = false);
	// Remove a sound from the map to free up memory
	void UnloadSound(const string& soundName);
	// "pos" will be the position of the camera's transform
	//void Set3DListenerAndOrientation(const glm::dvec3& pos = glm::dvec3(0.0, 0.0, 0.0), float volume_dB = 0.0f);
	// "pos" will be the position of the unit/structure's transform
	int PlaySound(const string& soundName, const glm::dvec3& pos = glm::dvec3(0.0, 0.0, 0.0), float volume_dB = 0.0f);
//	void PlaySoundOnThread(const string& soundName, const glm::dvec3& pos = glm::dvec3(0.0, 0.0, 0.0), float volume_dB = 0.0f);
	// Pause an indvidual sound 
	void StopChannel(int channelID);
	// Pause all sounds
	void StopAllChannels();
	void SetChannel3DPosition(int channelID, const glm::dvec3& pos);
	void SetChannelVolume(int channelID, float volume_dB);
	//bool IsPlaying(int channelID) const;
	float dB_to_Volume(float dB);
	float Volume_to_dB(float volume);
	FMOD_VECTOR VectorToFMOD(const glm::dvec3& pos);
};