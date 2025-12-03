#include <Console.h>

#include <windows.h>
#include <Render.h>
#include <Engine.h>
#include "Utils.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	gce::GameManager::Create();
	gce::Console::Init();

	gce::Scene& scene1 = gce::Scene::Create();

	gce::GameObject& cameraObject = gce::GameObject::Create(scene1);
	cameraObject.transform.LocalTranslate({ 0, 0, -10 });

	gce::Camera* pCamera = cameraObject.AddComponent<gce::Camera>();

	pCamera->SetMainCamera();
	pCamera->SetType(gce::PERSPECTIVE);

	pCamera->perspective.fov = XM_PIDIV4;
	pCamera->perspective.nearPlane = 0.001f;
	pCamera->perspective.farPlane = 500.0f;
	pCamera->perspective.aspectRatio = 1000.0f / 800.0f;
	pCamera->perspective.up = { 0.0f, 1.0f, 0.0f };

	ImportBlenderScene();

	gce::WindowParam param;

	param.width = 1920;
	param.height = 1080;
	param.title = L"Arena Shooter";
	param.isFullScreen = false;
	param.isSplitScreen = false;

	gce::GameManager::Run(param);
	gce::GameManager::Destroy();

	return 0;
}

