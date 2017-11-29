//#include "SoundManager.h"
//#include "SoundEmitter.h"
//
//void SoundManager::Initilaise()
//{
//	device = alcOpenDevice(NULL);
//	if (!device)
//		ErrorMessage("No sound device");
//	context = alcCreateContext(device, NULL);
//	alcMakeContextCurrent(context);
//	if (!context)
//		ErrorMessage("No sound context");
//	numOfEmitters = 1;
//}
//
//int SoundManager::GetNumOfEmitters()
//{
//	return numOfEmitters;
//}
//
//void SoundManager::StoreEmitter(SoundEmitter& newEmitter)
//{
//	emitterList.push_back(newEmitter);
//	numOfEmitters++;
//}
//
//SoundEmitter& SoundManager::GetEmitter(SoundEmitter& searchEmitter)
//{
//	// Use const_iterator?
//}
//
//void SoundManager::CleanUp()
//{
//	for (SoundEmitter &e : emitterList)
//	{
//		fclose(e.fp);
//		delete[] e.data;
//		alDeleteSources(1, &e.source);
//		alDeleteBuffers(1, &e.buffer);
//	}
//	emitterList.clear();
//	alcMakeContextCurrent(NULL);
//	alcDestroyContext(context);
//	alcCloseDevice(device);
//}
//
//void SoundManager::ErrorMessage(char* msg)
//{
//	// Display error message in console
//	std::cout << msg << std::endl;
//	return;
//}