#pragma once
#include "Singleton.h"
#include "Renderable.h"
#include <string>
#include <fstream>
#include <ostream>
class FileIO : public Singleton<FileIO>
{
private:
	void ConfigureGame(std::string para);

public:
	// Returns true if file exists.
	bool FileExists(std::string &filePath);

	// Loads in the ini file for startup. If no file exists, create a default configuration.
	bool LoadIniFile();

	// Create a default ini file.
	bool CreateIniFile();

	// Save current configuration to ini file.
	bool SaveIniFile();

	
};