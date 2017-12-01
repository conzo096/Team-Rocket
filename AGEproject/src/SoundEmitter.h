#pragma once
#include <AL\al.h>
#include <AL\alc.h>
#include <vector>
#include <iostream>
#include "glm\glm.hpp"
#include "Entity.h"

//class SoundEmitter : public Component
//{
//private:
//	ALCdevice *device;
//	ALCcontext *context;
//	char type[4];
//	DWORD chunkSize, subchunk1Size, sampleRate, avgBytesPerSec, subchunk2Size;
//	short audioFormat, channels, bytesPerSample, bitsPerSample;
//	ALuint source;
//	ALuint buffer; 
//	unsigned char* data;
//
//public:
//	SoundEmitter() : Component("SoundEmitter") {}
//	~SoundEmitter() {}
//
//	void Initilaise();
//	void LoadSoundFile(const char* filepath);
//	void PlaySoundFile(glm::dvec3 sourcePos, glm::dvec3 sourceVel, 
//					   glm::dvec3 listenerPos, glm::dvec3 listenerVel, glm::dvec3 listenerFwd, glm::dvec3 listenerUp);
//	void ErrorMessage(char* msg);
//	void CleanUp();
//};