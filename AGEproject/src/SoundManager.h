//#pragma once
//#include <AL\alc.h>
//#include <vector>
//#include <iostream>
//#include <Windows.h>
//#include "Entity.h"
//#include "Singleton.h"
//
//class SoundEmitter;
//
//class SoundManager : public Singleton<SoundManager>
//{
//private:
//	int numOfEmitters;
//	ALCdevice *device;
//	ALCcontext *context;
//	std::vector<SoundEmitter&> emitterList;
//
//public:
//	void Initilaise();
//	int GetNumOfEmitters();
//	void StoreEmitter(SoundEmitter& newEmitter);
//	SoundEmitter& GetEmitter(SoundEmitter& newEmitter);
//	void ErrorMessage(char* msg);
//	void CleanUp();
//};