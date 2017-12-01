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

	int nextChannelID;

	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;

	SoundMap mSounds;
	ChannelMap mChannels;
};

class AudioEngine : public Singleton<AudioEngine>
{
public:
	void Initialise();
	void Update();
	void Shutdown();
	int ErrorCheck(FMOD_RESULT result);

	void LoadSound(const string& soundName, bool is3d = true, bool looping = false, bool stream = false);
	void UnloadSound(const string& soundName);
	// "pos" will be the position of the camera's transform
	//void Set3DListenerAndOrientation(const glm::dvec3& pos = glm::dvec3(0.0, 0.0, 0.0), float volume_dB = 0.0f);
	// "pos" will be the position of the unit/structure's transform
	int PlaySound(const string& soundName, const glm::dvec3& pos = glm::dvec3(0.0, 0.0, 0.0), float volume_dB = 0.0f);
//	void PlaySoundOnThread(const string& soundName, const glm::dvec3& pos = glm::dvec3(0.0, 0.0, 0.0), float volume_dB = 0.0f);
	//void StopChannel(int channelID);
	//void StopAllChannels();
	void SetChannel3DPosition(int channelID, const glm::dvec3& pos);
	void SetChannelVolume(int channelID, float volume_dB);
	//bool IsPlaying(int channelID) const;
	float dB_to_Volume(float dB);
	float Volume_to_dB(float volume);
	FMOD_VECTOR VectorToFMOD(const glm::dvec3& pos);
};