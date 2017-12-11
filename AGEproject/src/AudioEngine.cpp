#include "AudioEngine.h"
#include <vector>
#include <iostream>
#include <thread>

Implementation* imp = nullptr;



int p(const string& soundName, const glm::dvec3& pos, float volume_dB)
{
	int channelID = imp->nextChannelID++;
	auto foundIt = imp->mSounds.find(soundName);
	if (foundIt == imp->mSounds.end())
	{
		AudioEngine::Get().LoadSound(soundName);
		foundIt = imp->mSounds.find(soundName);
		if (foundIt == imp->mSounds.end())
		{
			return channelID;
		}
	}
	FMOD::Channel* pChannel = nullptr;
	AudioEngine::Get().ErrorCheck(imp->audioSystem->playSound(foundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		foundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) {
			FMOD_VECTOR position = AudioEngine::Get().VectorToFMOD(pos);
			AudioEngine::Get().ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		AudioEngine::Get().ErrorCheck(pChannel->setVolume(AudioEngine::Get().dB_to_Volume(volume_dB)));
		AudioEngine::Get().ErrorCheck(pChannel->setPaused(false));
		imp->mChannels[channelID] = pChannel;
	}
	return channelID;
}





// Initialise FMOD
Implementation::Implementation()
{
	audioSystem = NULL;
	AudioEngine::Get().ErrorCheck(FMOD::System_Create(&audioSystem));
	audioSystem->init(32, FMOD_INIT_NORMAL, 0);
}

// Clean up FMOD
Implementation::~Implementation()
{
	AudioEngine::Get().ErrorCheck(audioSystem->release());
}

// Check for unused audio channels and close them
void Implementation::Update()
{
	vector<ChannelMap::iterator> stoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool isPlaying = false;
		it->second->isPlaying(&isPlaying);
		if (!isPlaying)
		{
			stoppedChannels.push_back(it);
		}
	}
	for (auto &it : stoppedChannels)
	{
		mChannels.erase(it);
	}
	AudioEngine::Get().ErrorCheck(audioSystem->update());
}

void AudioEngine::Initialise() {
	imp = new Implementation;
}

void AudioEngine::Update() {
	imp->Update();
}

// Load a sound file, set its attributes and store it in map
void AudioEngine::LoadSound(const string& soundName, bool is3d, bool looping, bool stream)
{
	auto foundIt = imp->mSounds.find(soundName);
	if (foundIt != imp->mSounds.end())
		return;

	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= is3d ? FMOD_3D : FMOD_2D;
	eMode |= looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* sound = nullptr;
	AudioEngine::Get().ErrorCheck(imp->audioSystem->createSound(soundName.c_str(), eMode, nullptr, &sound));
	if (sound) {
		imp->mSounds[soundName] = sound;
	}
}

void AudioEngine::UnloadSound(const string& soundName)
{
	auto foundIt = imp->mSounds.find(soundName);
	if (foundIt == imp->mSounds.end())
		return;

	AudioEngine::Get().ErrorCheck(foundIt->second->release());
	imp->mSounds.erase(foundIt);
}

int AudioEngine::PlaySound(const string& soundName, const glm::dvec3& pos, float volume_dB)
{
	int channelID = imp->nextChannelID++;
	auto foundIt = imp->mSounds.find(soundName);
	if (foundIt == imp->mSounds.end())
	{
		LoadSound(soundName);
		foundIt = imp->mSounds.find(soundName);
		if (foundIt == imp->mSounds.end())
		{
			return channelID;
		}
	}
	FMOD::Channel* pChannel = nullptr;
	AudioEngine::Get().ErrorCheck(imp->audioSystem->playSound(foundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		foundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) {
			FMOD_VECTOR position = VectorToFMOD(pos);
			AudioEngine::Get().ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		AudioEngine::Get().ErrorCheck(pChannel->setVolume(dB_to_Volume(volume_dB)));
		AudioEngine::Get().ErrorCheck(pChannel->setPaused(false));
		imp->mChannels[channelID] = pChannel;
	}
	return channelID;
}

void AudioEngine::StopChannel(int channelID)
{
	FMOD::Channel* sChannel = nullptr;
	sChannel = imp->mChannels[channelID];
	AudioEngine::Get().ErrorCheck(sChannel->setPaused(true));
}

void AudioEngine::StopAllChannels()
{
	for (auto sc : imp->mChannels)
	{
		AudioEngine::Get().StopChannel(sc.first);
	}
}

void AudioEngine::PlaySoundOnThread(const string& soundName, const glm::dvec3& pos, float volume_dB)
{
	std::thread t(p,soundName, pos, volume_dB);
	t.join();
}

void AudioEngine::SetChannel3DPosition(int channelID, const glm::dvec3& pos)
{
	auto foundIt = imp->mChannels.find(channelID);
	if (foundIt == imp->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFMOD(pos);
	AudioEngine::Get().ErrorCheck(foundIt->second->set3DAttributes(&position, NULL));
}

void AudioEngine::SetChannelVolume(int channelID, float volume_dB)
{
	auto foundIt = imp->mChannels.find(channelID);
	if (foundIt == imp->mChannels.end())
		return;
	AudioEngine::ErrorCheck(foundIt->second->setVolume(dB_to_Volume(volume_dB)));
}

float AudioEngine::dB_to_Volume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float AudioEngine::Volume_to_dB(float volume)
{
	return 20.0f * log10f(volume);
}

FMOD_VECTOR AudioEngine::VectorToFMOD(const glm::dvec3& pos) {
	FMOD_VECTOR fVec;
	fVec.x = pos.x;
	fVec.y = pos.y;
	fVec.z = pos.z;
	return fVec;
}

int AudioEngine::ErrorCheck(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		cout << "FMOD error " << result << endl;
		return 1;
	}
	//cout << "FMOD all good!" << endl;
	return 0;
}

void AudioEngine::Shutdown() {
	delete imp;
}