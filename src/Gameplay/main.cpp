#include <Console.h>

#include <windows.h>
#include <Render.h>
#include <Engine.h>

#include "Prefabs/ArenaCamera.h"
#include "Scripts/PlayerBehavior.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	gce::GameManager::Create();
	gce::Console::Init();

	gce::Scene& scene1 = gce::Scene::Create();

	ArenaCamera ac;
	ac.Create(scene1);
	ac.SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);

	gce::WindowParam param;
	param.width = 1920;
	param.height = 1080;
	param.title = L"Arena Shooter";
	param.isFullScreen = false;
	param.isSplitScreen = false;

	static gce::D12PipelineObject pso = {
	gce::SHADERS.VERTEX,
	gce::SHADERS.PIXEL,
	gce::SHADERS.HULL,
	gce::SHADERS.DOMAIN_,
	gce::SHADERS.ROOT_SIGNATURE
	};

	gce::Texture* pRockTexture = new gce::Texture("res/Assets/rock/rock_base_color.png");

	LightManager::SetLightsProperties(8.0f, 100.0f, 2.0f, 32.0f, 1.f);
	LightData directionalLight = LightManager::CreateDirectionalLight(Vector3f32(0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
	LightManager::AddLight(directionalLight);

	gce::GameObject& player = gce::GameObject::Create(scene1);
	player.SetName("Player");

	gce::PhysicComponent* pPhysic = player.AddComponent<gce::PhysicComponent>();
	gce::MeshRenderer* pPlayerMesh = player.AddComponent<gce::MeshRenderer>();
	pPlayerMesh->pGeometry = gce::SHAPES.CUBE;

	pPlayerMesh->pMaterial->albedoTextureID = pRockTexture->GetTextureID();
	pPlayerMesh->pMaterial->useTextureAlbedo = 1;
	pPlayerMesh->pMaterial->subsurface = 1;
	pPlayerMesh->pPso = &pso;

	gce::BoxCollider* sCollider = player.AddComponent<gce::BoxCollider>();

	PlayerBehavior* pScript = player.AddScript<PlayerBehavior>();
	pScript->SetCam(&ac);

	///

	gce::GameObject& floor = gce::GameObject::Create(scene1);
	floor.transform.SetLocalScale({ 10, 1, 10 });
	floor.transform.SetWorldPosition({ 0, -2, 0 });
	floor.SetName("Floor");
	gce::MeshRenderer* pFloorMesh = floor.AddComponent<gce::MeshRenderer>();
	pFloorMesh->pGeometry = gce::SHAPES.CUBE;

	pFloorMesh->pMaterial->albedoTextureID = pRockTexture->GetTextureID();
	pFloorMesh->pMaterial->useTextureAlbedo = 1;
	pFloorMesh->pMaterial->subsurface = 1;
	pFloorMesh->pPso = &pso;

	BoxCollider* pFloorBox = floor.AddComponent<BoxCollider>();

	gce::GameManager::Run(param);
	gce::GameManager::Destroy();

	return 0;
}

