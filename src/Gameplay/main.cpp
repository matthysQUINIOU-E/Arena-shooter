#include <Console.h>

#include <windows.h>
#include <Render.h>
#include <Engine.h>
#include "SceneManager.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	gce::GameManager::Create();
	gce::Console::Init();
	SceneManager sceneM;
	sceneM.Init();
	gce::GameManager::Destroy();

	return 0;
}

