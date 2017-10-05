#include "FileIO.h"
#include "UserControls.h"

bool FileIO::FileExists(std::string &filePath)
{
	std::ifstream file(filePath);
	return file.good();
}


bool FileIO::LoadIniFile()
{
	if (!FileExists(std::string("test.ini")))
		CreateIniFile();
	// Start reading and setting parameters//
	std::ifstream file("test.ini");
	std::string para;
	while (std::getline(file, para))
	{
		ConfigureGame(para);
	}

	return true;
}

bool FileIO::CreateIniFile()
{
	std::ofstream outputFile;
	outputFile.open("test.ini");
	outputFile << "Window options" << std::endl;
	outputFile << "FullScreen=0" << std::endl;
	outputFile << "Width=1920" << std::endl;
	outputFile << "Height=1080" << std::endl;

	outputFile << "Controller bindings" << std::endl;
	outputFile << "Action=0" << std::endl;
	outputFile << "Forward=87" << std::endl;
	outputFile << "Backward=83" << std::endl;
	outputFile << "Left=65" << std::endl;
	outputFile << "Right=68" << std::endl;
	outputFile << "ResetCamera=32" << std::endl;
	outputFile << "Escape=256" << std::endl;
	outputFile << "RotateLeft=81" << std::endl;
	outputFile << "RotateRight=69" << std::endl;
	outputFile << "Up=32" << std::endl;
	outputFile << "Down=341" << std::endl;
	outputFile << "Enter=257" << std::endl;
	outputFile.close();

	return true;
}


void FileIO::ConfigureGame(std::string para)
{
	std::string delimiter = "=";
	if (para.find(delimiter))
	{
		std::string token = para.substr(0, para.find(delimiter));
		std::string subString = para.substr(para.find(delimiter)+1, para.length());
		if (subString == token)
			return;
		unsigned int value = std::stoi(subString);
		// First check if string meets critera.
		if (token == "FullScreen")
			GameEngine::Instance()->SetFullScreen(value);
		if (token == "Width")
			GameEngine::Instance()->SetScreenWidth(value);
		if (token == "Height")
			GameEngine::Instance()->SetScreenHeight(value);

		
		if (token == "Action")
			UserControls::get().BindKey(token, value);
		if (token == "Forward")
			UserControls::get().BindKey(token, value);
		if (token == "Backward")
			UserControls::get().BindKey(token, value);
		if (token == "Left")
			UserControls::get().BindKey(token, value);
		if (token == "Right")
			UserControls::get().BindKey(token, value);
		if (token == "ResetCamera")
			UserControls::get().BindKey(token, value);
		if (token == "Escape")
			UserControls::get().BindKey(token, value);
		if (token == "RotateLeft")
			UserControls::get().BindKey(token, value);
		if (token == "RotateRight")
			UserControls::get().BindKey(token, value);
		if (token == "Up")
			UserControls::get().BindKey(token, value);
		if (token == "Down")
			UserControls::get().BindKey(token, value);
		if (token == "Enter")
			UserControls::get().BindKey(token, value);
	}
	else
		std::cout << "Error" << std::endl;
}


