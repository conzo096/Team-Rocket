//#pragma once
//#include <AL\al.h>
//#include "SoundManager.h"
//
//class SoundEmitter : public Component
//{
//private:
//	char type[4];
//	DWORD chunkSize, subchunk1Size, sampleRate, avgBytesPerSec, subchunk2Size;
//	short audioFormat, channels, bytesPerSample, bitsPerSample;
//
//public:
//	SoundEmitter() : Component("SoundEmitter" + SoundManager::Get().GetNumOfEmitters()) {}
//	~SoundEmitter() {}
//
//	FILE *fp;
//	ALuint source;
//	ALuint buffer; 
//	unsigned char* data;
//
//	void LoadSoundFile(const char* filepath);
//	void PlaySoundFile(glm::dvec3 sourcePos, glm::dvec3 sourceVel, 
//					   glm::dvec3 listenerPos, glm::dvec3 listenerVel, glm::dvec3 listenerFwd, glm::dvec3 listenerUp);
//	void ErrorMessage(char* msg);
//};