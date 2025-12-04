#include <Console.h>

#include <windows.h>
#include <Render.h>
#include <Engine.h>
#include "SceneManager.h"

#include "Prefabs/ArenaCamera.h"
#include "Scripts/PlayerBehavior.hpp"
#include "Scripts/CameraBehavior.hpp"
#include "Scripts/FpsBehavior.hpp"
#include <iostream>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	gce::GameManager::Create();
	gce::Console::Init();
	SceneManager sceneM;

	gce::WindowParam param;
	param.width = WINDOW_WIDTH;
	param.height = WINDOW_HEIGHT;
	param.title = L"Arena Shooter";
	param.isFullScreen = false;
	param.isSplitScreen = false;

	gce::GameManager::Run(param);
	gce::GameManager::Destroy();

	return 0;
}

