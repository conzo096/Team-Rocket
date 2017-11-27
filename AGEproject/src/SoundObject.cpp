#include "SoundObject.h"

using namespace std;

void SoundObject::Initilaise()
{
	device = alcOpenDevice(NULL);
	if (!device)
		ErrorMessage("No sound device");
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	if (!context)
		ErrorMessage("No sound context");
}

void SoundObject::LoadSoundFile(const char* filename)
{
	fp = NULL;
	fp = fopen(filename, "rb");

	// Go to http://soundfile.sapp.org/doc/WaveFormat/ 
	// for breakdown of .wav file format
	fread(type, sizeof(char), 4, fp);
	string chunkID(type);
	if (!(chunkID == "RIFF")) { ErrorMessage("No RIFF"); }

	fread(&chunkSize, sizeof(DWORD), 1, fp);

	fread(type, sizeof(char), 4, fp);
	string format(type);
	if (!(format == "WAVE")) { ErrorMessage("No WAVE"); }

	fread(type, sizeof(char), 4, fp);
	string subchunk1ID(type);
	if (!(subchunk1ID == "fmt ")) { ErrorMessage("No fmt"); }

	fread(&subchunk1Size, sizeof(DWORD), 1, fp);

	fread(&audioFormat, sizeof(short), 1, fp);

	fread(&channels, sizeof(short), 1, fp);

	fread(&sampleRate, sizeof(DWORD), 1, fp);

	fread(&avgBytesPerSec, sizeof(DWORD), 1, fp);

	fread(&bytesPerSample, sizeof(short), 1, fp);

	fread(&bitsPerSample, sizeof(short), 1, fp);

	fread(type, sizeof(char), 4, fp);
	string subchunk2ID(type);
	if (!(subchunk2ID == "data")) { ErrorMessage("No data"); }

	fread(&subchunk2Size, sizeof(DWORD), 1, fp);

	data = new unsigned char[subchunk2Size];
	fread(data, sizeof(BYTE), subchunk2Size, fp);
}

void SoundObject::PlaySoundFile(glm::dvec3 sourcePos, glm::dvec3 sourceVel,
							     glm::dvec3 listenerPos, glm::dvec3 listenerVel, glm::dvec3 listenerOri)
{
	ALuint frequency = sampleRate;
	ALenum format = 0;

	alGenBuffers(1, &buffer);
	alGenSources(1, &source);

	switch (channels)
	{
	case 1:
		if (bitsPerSample == 8)
			format = AL_FORMAT_MONO8;
		else if (bitsPerSample == 16)
			format = AL_FORMAT_MONO16;
		break;
	case 2:
		if (bitsPerSample == 8)
			format = AL_FORMAT_STEREO8;
		else if (bitsPerSample == 16)
			format = AL_FORMAT_STEREO16;
		break;
	}

	alBufferData(buffer, format, data, subchunk2Size, frequency);

	ALfloat sp[] = { sourcePos.x, sourcePos.y, sourcePos.z };
	ALfloat sv[] = { sourceVel.x, sourceVel.y, sourceVel.z };
	ALfloat lp[] = { listenerPos.x, listenerPos.y, listenerPos.z };
	ALfloat lv[] = { listenerVel.x, listenerVel.y, listenerVel.z };
	ALfloat lo[] = { listenerOri.x, listenerOri.y, listenerOri.z };

	// Listener
	alListenerfv(AL_POSITION, lp);
	alListenerfv(AL_ORIENTATION, lo);
	alListenerfv(AL_VELOCITY, lv);

	// Source
	alSourcei(source, AL_BUFFER, buffer);
	alSourcef(source, AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 1.0f);
	alSourcefv(source, AL_POSITION, sp);
	alSourcefv(source, AL_VELOCITY, sv);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	alSourcePlay(source);
}

void SoundObject::ErrorMessage(char* msg)
{
	// Display error message in console
	cout << msg << endl;
	return;
}

void SoundObject::CleanUp()
{
	fclose(fp);
	delete[] data;
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}