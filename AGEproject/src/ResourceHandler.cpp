#include "ResourceHandler.h"

void ResourceHandler::LoadModels()
{
	std::cout << "Convert to file loading!" << std::endl;
	models.insert(std::pair<std::string, Model*>(std::string("Resource"), new Model("../res/models/Resource.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("Worker"), new Model("../res/models/Worker.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("DronePlatform"), new Model("../res/models/DronePlatform.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("DroneTurret"), new Model("../res/models/DroneTurret.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("WardenPlatform"), new Model("../res/models/WardenPlatform.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("WardenTurret"), new Model("../res/models/WardenTurret.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("Kestrel"), new Model("../res/models/Flyer.obj")));

	models.insert(std::pair<std::string, Model*>(std::string("Base"), new Model("../res/models/Constructor.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("Factory"), new Model("../res/models/Factory.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("VehicleBay"), new Model("../res/models/VehicleBay.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("Hanger"), new Model("../res/models/Hanger.obj")));

	models.insert(std::pair<std::string, Model*>(std::string("Torus"), new Model("../res/models/Torus2.obj")));
	models.insert(std::pair<std::string, Model*>(std::string("Plane"), GeometryUtil::BuildPlane(1, 100, 100)));
	models.insert(std::pair<std::string, Model*>(std::string("BillBoard"), GeometryUtil::BuildPlane(0.5, 4,4)));
}

void ResourceHandler::LoadTextures()
{
	std::cout << "Convert to file loading!" << std::endl;
	textures.insert(std::pair<std::string, unsigned int>(std::string("ResourceUV"), Texture("../res/textures/ResourceUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("WorkerUV"), Texture("../res/textures/WorkerUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("ConstructorUV"), Texture("../res/textures/ConstructorUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("DronePlatformUV"), Texture("../res/textures/DronePlatformUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("DroneTurretUV"), Texture("../res/textures/DroneTurretUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("WardenPlatformUV"), Texture("../res/textures/WardenPlatformUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("WardenTurretUV"), Texture("../res/textures/WardenTurretUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("FlyerUV"), Texture("../res/textures/FlyerUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("FactoryUV"), Texture("../res/textures/FactoryUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("VehicleBayUV"), Texture("../res/textures/VehicleBayUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("HangerUV"), Texture("../res/textures/HangerUV.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("debug"), Texture("../res/textures/debug.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Rock"), Texture("../res/textures/rock.png").GetTextureId()));

	// MainMenu buttons and labels
	textures.insert(std::pair<std::string, unsigned int>(std::string("Start_Game"), Texture("../res/textures/UI/MainMenu/MainMenu_Button1.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Options"), Texture("../res/textures/UI/MainMenu/MainMenu_Button2.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Exit_Game"), Texture("../res/textures/UI/MainMenu/MainMenu_Button3.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Start_Game_HIGHLIGHTED"), Texture("../res/textures/UI/MainMenu/MainMenu_Button1_Highlighted.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Options_HIGHLIGHTED"), Texture("../res/textures/UI/MainMenu/MainMenu_Button2_Highlighted.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Exit_Game_HIGHLIGHTED"), Texture("../res/textures/UI/MainMenu/MainMenu_Button3_Highlighted.png").GetTextureId()));
	
	// SettingsMenu buttons and labels
	textures.insert(std::pair<std::string, unsigned int>(std::string("Resolution"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_Title1.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Screen_mode"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_Title2.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("1920x1080"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_Option1-1.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("1600x900"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_Option1-2.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("1280x720"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_Option1-3.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("1024x576"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_Option1-4.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Windowed"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_Option2-2.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Fullscreen"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_Option2-1.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Customise_controls"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_LargeButton1.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Save_changes"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_LargeButton2.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Cancel"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_LargeButton3.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Left_arrow"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_SmallButton1.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Right_arrow"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_SmallButton2.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Customise_controls_HIGHLIGHTED"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_LargeButton1_Highlighted.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Save_changes_HIGHLIGHTED"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_LargeButton2_Highlighted.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Cancel_HIGHLIGHTED"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_LargeButton3_Highlighted.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Left_arrow_HIGHLIGHTED"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_SmallButton1_Highlighted.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Right_arrow_HIGHLIGHTED"), Texture("../res/textures/UI/SettingsMenu/SettingsMenu_SmallButton2_Highlighted.png").GetTextureId()));

	// Pause Menu buttons
	textures.insert(std::pair<std::string, unsigned int>(std::string("Save_Game"), Texture("../res/textures/UI/SettingsMenu/Save_Game_Button.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Load_Game"), Texture("../res/textures/UI/SettingsMenu/Load_Game_Button.png").GetTextureId()));

	// ControlsMenu buttons
	textures.insert(std::pair<std::string, unsigned int>(std::string("Forward"), Texture("../res/textures/UI/ControlsMenu/Forward.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Backward"), Texture("../res/textures/UI/ControlsMenu/Backward.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Left"), Texture("../res/textures/UI/ControlsMenu/Pan_Left.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Right"), Texture("../res/textures/UI/ControlsMenu/Pan_Right.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("RotateLeft"), Texture("../res/textures/UI/ControlsMenu/Rotate_Left.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("RotateRight"), Texture("../res/textures/UI/ControlsMenu/Rotate_Right.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("ZoomIn"), Texture("../res/textures/UI/ControlsMenu/Zoom_In.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("ZoomOut"), Texture("../res/textures/UI/ControlsMenu/Zoom_Out.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Hold"), Texture("../res/textures/UI/ControlsMenu/Hold.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("HotKey1"), Texture("../res/textures/UI/ControlsMenu/Hotkey_1.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("HotKey2"), Texture("../res/textures/UI/ControlsMenu/Hotkey_2.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("HotKey3"), Texture("../res/textures/UI/ControlsMenu/Hotkey_3.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Select"), Texture("../res/textures/UI/ControlsMenu/Select.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Reset"), Texture("../res/textures/UI/ControlsMenu/Reset.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Back"), Texture("../res/textures/UI/ControlsMenu/Back.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Forward_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Forward_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Backward_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Backward_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Left_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Pan_Left_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Right_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Pan_Right_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("RotateLeft_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Rotate_Left_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("RotateRight_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Rotate_Right_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("ZoomIn_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Zoom_In_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("ZoomOut_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Zoom_Out_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Hold_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Hold_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("HotKey1_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Hotkey_1_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("HotKey2_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Hotkey_2_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("HotKey3_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Hotkey_3_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Select_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Select_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Reset_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Reset_Highlight.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Back_HIGHLIGHTED"), Texture("../res/textures/UI/ControlsMenu/Back_Highlight.png").GetTextureId()));

	// UI font
	textures.insert(std::pair<std::string, unsigned int>(std::string("Font"), Texture("../res/textures/Holstein.png").GetTextureId()));

	// Splash Screen - M8 you get much better compression with jpeg for colour images but they break it, needs 4 channels apparently
	textures.insert(std::pair<std::string, unsigned int>(std::string("Splash_Screen"), Texture("../res/textures/UI/SplashScreen.png").GetTextureId()));
	textures.insert(std::pair<std::string, unsigned int>(std::string("Background"), Texture("../res/textures/UI/Background.png").GetTextureId()));
}

void ResourceHandler::LoadShaders()
{
	std::cout << "Convert to file loading!" << std::endl;

	GLShader* shader = new GLShader();
	if (!shader->AddShaderFromFile("../res/shaders/phong.vert", GLShader::VERTEX))
		printf("Vert failed to compile.\n");
	if (!shader->AddShaderFromFile("../res/shaders/phong.frag", GLShader::FRAGMENT))
		printf("Frag failed to compile.\n");
	shader->Link();
	shaders.insert(std::pair<std::string, GLShader*>("Phong", shader));
	GLShader* shader1 = new GLShader();
	if (!shader1->AddShaderFromFile("../res/shaders/Basic.vert", GLShader::VERTEX))
		printf("Vert failed to compile.\n");
	if (!shader1->AddShaderFromFile("../res/shaders/Basic.frag", GLShader::FRAGMENT))
		printf("Frag failed to compile.\n");
	shader1->Link();
	shaders.insert(std::pair<std::string, GLShader*>("Basic", shader1));

	GLShader* shader2 = new GLShader();
	if (!shader2->AddShaderFromFile("../res/shaders/point.vert", GLShader::VERTEX))
		printf("Vert failed to compile.\n");
	if (!shader2->AddShaderFromFile("../res/shaders/point.frag", GLShader::FRAGMENT))
		printf("Frag failed to compile.\n");
	shader->Link();
	shaders.insert(std::pair<std::string, GLShader*>("Point", shader2));
	GLShader* shader3 = new GLShader();
	if (!shader3->AddShaderFromFile("../res/shaders/Particle.vert", GLShader::VERTEX))
		printf("Vert failed to compile.\n");
	if (!shader3->AddShaderFromFile("../res/shaders/Particle.frag", GLShader::FRAGMENT))
		printf("Frag failed to compile.\n");
	shader3->Link();
	shaders.insert(std::pair<std::string, GLShader*>("Particle", shader3));

	GLShader* shader4 = new GLShader();
	if (!shader4->AddShaderFromFile("../res/shaders/font.vert", GLShader::VERTEX))
		printf("Vert failed to compile.\n");
	if (!shader4->AddShaderFromFile("../res/shaders/font.frag", GLShader::FRAGMENT))
		printf("Frag failed to compile.\n");
	shader4->Link();
	shaders.insert(std::pair<std::string, GLShader*>("Font", shader4));
}

void ResourceHandler::LoadAudio()
{
	audio.insert(std::pair<std::string, char*>(std::string("Advance"), "../res/audio/tone-beep.wav"));
	audio.insert(std::pair<std::string, char*>(std::string("Return"), "../res/audio/tone-beep-reverse.wav"));
	audio.insert(std::pair<std::string, char*>(std::string("Theme"), "../res/audio/Clash_Defiant.wav"));
}
