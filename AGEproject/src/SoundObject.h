#pragma once
#include <AL\al.h>
#include <AL\alc.h>
#include <iostream>
#include <Windows.h>
#include "glm\glm.hpp"

class SoundObject
{
private:
	FILE *fp;
	ALCdevice *device;
	ALCcontext *context;
	ALuint source;
	ALuint buffer;

public:
	SoundObject() {}
	~SoundObject() {}

	char type[4];
	DWORD chunkSize, subchunk1Size, sampleRate, avgBytesPerSec, subchunk2Size;
	short audioFormat, channels, bytesPerSample, bitsPerSample;
	unsigned char* data;
	
	void Initilaise();
	void LoadSoundFile(const char* filename);
	void PlaySoundFile(glm::dvec3 sourcePos, glm::dvec3 sourceVel, 
					   glm::dvec3 listenerPos, glm::dvec3 listenerVel, glm::dvec3 listenerOri);
	void ErrorMessage(char* msg);
	void CleanUp();
};