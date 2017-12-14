#include "FileIO.h"
#include "UserControls.h"

bool FileIO::FileExists(std::string &filePath)
{
	std::ifstream file(filePath);
	return file.good();
}

bool FileIO::LoadIniFile()
{
	UserControls::Get().ResetControllerBindings();
	if (!FileExists(std::string("../res/config.ini")))
	{
		std::cout << "Creating file at ../res" << std::endl;
		CreateIniFile();
	}
	// Start reading and setting parameters
	std::ifstream file("../res/config.ini");
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
	outputFile.open("../res/config.ini");
	outputFile << "Window options" << std::endl;
	outputFile << "FullScreen=0" << std::endl;
	outputFile << "Width=1920" << std::endl;
	outputFile << "Height=1080" << std::endl;

	outputFile << "Keyboard bindings" << std::endl;
	outputFile << "Action=0" << std::endl;
	outputFile << "Move=1" << std::endl;
	outputFile << "Forward=87" << std::endl;
	outputFile << "Backward=83" << std::endl;
	outputFile << "Left=65" << std::endl;
	outputFile << "Right=68" << std::endl;
	outputFile << "ResetCamera=32" << std::endl;
	outputFile << "Escape=256" << std::endl;
	outputFile << "RotateLeft=81" << std::endl;
	outputFile << "RotateRight=69" << std::endl;
	outputFile << "ZoomIn=32" << std::endl;
	outputFile << "ZoomOut=341" << std::endl;
	outputFile << "Enter=257" << std::endl;
	outputFile << "Hold=88" << std::endl;
	outputFile << "HotKey1=49" << std::endl;
	outputFile << "HotKey2=50" << std::endl;
	outputFile << "HotKey3=51" << std::endl;
	outputFile << "HotKey4=52" << std::endl;

	outputFile << "Controller bindings" << std::endl;
	outputFile << "A=0" << std::endl;
	outputFile << "B=1" << std::endl;
	outputFile << "X=2" << std::endl;
	outputFile << "Y=3" << std::endl;
	outputFile << "leftShoulder=4" << std::endl;
	outputFile << "rightShoulder=5" << std::endl;
	outputFile << "back=6" << std::endl;
	outputFile << "start=7" << std::endl;
	outputFile << "leftSticker=8" << std::endl;
	outputFile << "rightSticker=9" << std::endl;
	outputFile << "dUp=10" << std::endl;
	outputFile << "dRight=11" << std::endl;
	outputFile << "dDown=12" << std::endl;
	outputFile << "dLeft=13" << std::endl;

	outputFile.close();

	return true;
}



bool FileIO::SaveIniFile()
{
	std::ofstream outputFile;
	outputFile.open("../res/config.ini");
	outputFile << "Window options" << std::endl;
	outputFile << "FullScreen=" <<GameEngine::Get().GetFullScreen() << std::endl;
	outputFile << "Width="<<GameEngine::Get().GetScreenWidth() << std::endl;
	outputFile << "Height="<<GameEngine::Get().GetScreenHeight() << std::endl;

	outputFile << "Keyboard bindings" << std::endl;
	outputFile << "Action=" << UserControls::Get().GetKeyValue("Action") << std::endl;
	outputFile << "Move=" <<  UserControls::Get().GetKeyValue("Move") << std::endl;
	outputFile << "Forward=" <<  UserControls::Get().GetKeyValue("Forward") << std::endl;
	outputFile << "Backward=" <<  UserControls::Get().GetKeyValue("Backward") << std::endl;
	outputFile << "Left=" <<  UserControls::Get().GetKeyValue("Left") << std::endl;
	outputFile << "Right=" <<  UserControls::Get().GetKeyValue("Right") << std::endl;
//	outputFile << "ResetCamera=" <<  UserControls::Get().GetKeyValue("ResetCamera") << std::endl;
	outputFile << "Escape=" <<  UserControls::Get().GetKeyValue("Escape") << std::endl;
	outputFile << "RotateLeft=" <<  UserControls::Get().GetKeyValue("RotateLeft") << std::endl;
	outputFile << "RotateRight=" <<  UserControls::Get().GetKeyValue("RotateRight") << std::endl;
	outputFile << "ZoomIn="<< UserControls::Get().GetKeyValue("ZoomIn") << std::endl;
	outputFile << "ZoomOut=" <<  UserControls::Get().GetKeyValue("ZoomOut") << std::endl;
	outputFile << "Enter=" <<  UserControls::Get().GetKeyValue("Enter") << std::endl;
	outputFile << "Hold=" <<  UserControls::Get().GetKeyValue("Hold") << std::endl;
	outputFile << "HotKey1=" <<  UserControls::Get().GetKeyValue("HotKey1") << std::endl;
	outputFile << "HotKey2=" <<  UserControls::Get().GetKeyValue("HotKey2") << std::endl;
	outputFile << "HotKey3=" <<  UserControls::Get().GetKeyValue("HotKey3") << std::endl;
	outputFile << "HotKey4=" << UserControls::Get().GetKeyValue("HotKey4") << std::endl;
	
	outputFile << "Controller bindings" << std::endl;
	outputFile << "A=" << UserControls::Get().GetControllerButton("A") << std::endl;
	outputFile << "B=" << UserControls::Get().GetControllerButton("B") << std::endl;
	outputFile << "X=" << UserControls::Get().GetControllerButton("X") << std::endl;
	outputFile << "Y=" << UserControls::Get().GetControllerButton("Y") << std::endl;
	outputFile << "leftShoulder=" << UserControls::Get().GetControllerButton("leftShoulder") << std::endl;
	outputFile << "rightShoulder=" << UserControls::Get().GetControllerButton("rightShoulder") << std::endl;
	outputFile << "back=" << UserControls::Get().GetControllerButton("back") << std::endl;
	outputFile << "start=" << UserControls::Get().GetControllerButton("start") << std::endl;
	outputFile << "leftSticker=" << UserControls::Get().GetControllerButton("leftSticker") << std::endl;
	outputFile << "rightSticker=" << UserControls::Get().GetControllerButton("rightSticker") << std::endl;
	outputFile << "dUp=" << UserControls::Get().GetControllerButton("dUp") << std::endl;
	outputFile << "dRight=" << UserControls::Get().GetControllerButton("dRight") << std::endl;
	outputFile << "dDown=" << UserControls::Get().GetControllerButton("dDown") << std::endl;
	outputFile << "dLeft=" << UserControls::Get().GetControllerButton("dLeft") << std::endl;

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
			GameEngine::Get().SetFullScreen(value);
		if (token == "Width")
			GameEngine::Get().SetScreenWidth(value);
		if (token == "Height")
			GameEngine::Get().SetScreenHeight(value);

		// Simplier way of approaching this?
		if (token == "Action")
			UserControls::Get().BindKey(token, value);
		if (token == "Move")
			UserControls::Get().BindKey(token, value);
		if (token == "Forward")
			UserControls::Get().BindKey(token, value);
		if (token == "Backward")
			UserControls::Get().BindKey(token, value);
		if (token == "Left")
			UserControls::Get().BindKey(token, value);
		if (token == "Right")
			UserControls::Get().BindKey(token, value);
		if (token == "ResetCamera")
			UserControls::Get().BindKey(token, value);
		if (token == "Escape")
			UserControls::Get().BindKey(token, value);
		if (token == "RotateLeft")
			UserControls::Get().BindKey(token, value);
		if (token == "RotateRight")
			UserControls::Get().BindKey(token, value);
		if (token == "ZoomIn")
			UserControls::Get().BindKey(token, value);
		if (token == "ZoomOut")
			UserControls::Get().BindKey(token, value);
		if (token == "Enter")
			UserControls::Get().BindKey(token, value);
		if (token == "Hold")
			UserControls::Get().BindKey(token, value);
		if (token == "HotKey1")
			UserControls::Get().BindKey(token, value);
		if (token == "HotKey2")
			UserControls::Get().BindKey(token, value);
		if (token == "HotKey3")
			UserControls::Get().BindKey(token, value);
		if (token == "HotKey4")
			UserControls::Get().BindKey(token, value);

		// Simplier way of approaching this?
		if (token == "A")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "B")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "X")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "Y")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "leftShoulder")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "rightShoulder")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "back")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "start")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "leftSticker")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "rightSticker")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "dUp")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "dRight")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "dDown")
			UserControls::Get().BindControllerButton(token, value);
		if (token == "dLeft")
			UserControls::Get().BindControllerButton(token, value);

	}
	else
		std::cout << "Error" << std::endl;
}


