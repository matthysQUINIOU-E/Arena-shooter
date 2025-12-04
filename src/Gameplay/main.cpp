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
	sceneM.Init();

	gce::Scene& scene1 = gce::Scene::Create();

	ArenaCamera ac;
	ac.Create(scene1);
	ac.SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);

	gce::WindowParam param;
	param.width = WINDOW_WIDTH;
	param.height = WINDOW_HEIGHT;
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

	gce::Texture* pWhiteTexture = new gce::Texture("res/Assets/white.png");

	LightManager::SetLightsProperties(8.0f, 100.0f, 2.0f, 32.0f, 1.f);
	LightData directionalLight = LightManager::CreateDirectionalLight(Vector3f32(0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
	LightManager::AddLight(directionalLight);

	gce::GameObject& player = gce::GameObject::Create(scene1);
	player.SetName("Player");
	player.transform.SetWorldScale({ 1, 2, 1 });
	gce::PhysicComponent* pPhysic = player.AddComponent<gce::PhysicComponent>();
	gce::MeshRenderer* pPlayerMesh = player.AddComponent<gce::MeshRenderer>();
	pPlayerMesh->pGeometry = gce::SHAPES.CUBE;

	pPlayerMesh->pMaterial->albedoTextureID = pWhiteTexture->GetTextureID();
	pPlayerMesh->pMaterial->useTextureAlbedo = 1;
	pPlayerMesh->pMaterial->subsurface = 1;
	pPlayerMesh->pPso = &pso;

	gce::BoxCollider* bCollider = player.AddComponent<gce::BoxCollider>();

	PlayerBehavior* pScript = player.AddScript<PlayerBehavior>();

	ac.GetScript<CameraBehavior>()->SetGameObjectToFollow(&player);

	Texture* pArmsBaseColor = new Texture("res/Assets/zhu_rong_arms/Arms_Base_Color.png");

	gce::GameObject& playerArms = gce::GameObject::Create(scene1);
	playerArms.SetName("Player_Arms");
	playerArms.transform.SetLocalPosition(ac.GetGameObject()->transform.GetWorldPosition());
	playerArms.transform.SetWorldScale({ 3, 3, 3 });
	gce::MeshRenderer* pArmsMesh = playerArms.AddComponent<gce::MeshRenderer>();
	pArmsMesh->pGeometry = gce::GeometryFactory::LoadGeometry("res/Assets/zhu_rong_arms/Arms.obj");
	pArmsMesh->pMaterial->albedoTextureID = pArmsBaseColor->GetTextureID();
	pArmsMesh->pMaterial->useTextureAlbedo = 1;
	pArmsMesh->pMaterial->subsurface = 1;
	pArmsMesh->pPso = &pso;

	ac.GetGameObject()->AddChild(playerArms);

	gce::GameObject& floor = gce::GameObject::Create(scene1);
	floor.transform.SetLocalScale({ 50, 1, 50 });
	floor.transform.SetWorldPosition({ 0, -2, 0 });
	floor.SetName("Floor");
	gce::MeshRenderer* pFloorMesh = floor.AddComponent<gce::MeshRenderer>();
	pFloorMesh->pGeometry = gce::SHAPES.CUBE;

	pFloorMesh->pMaterial->albedoTextureID = pWhiteTexture->GetTextureID();
	pFloorMesh->pMaterial->useTextureAlbedo = 1;
	pFloorMesh->pMaterial->subsurface = 1;
	pFloorMesh->pPso = &pso;

	BoxCollider* pFloorBox = floor.AddComponent<BoxCollider>();


	gce::GameObject& fps = gce::GameObject::Create(scene1);
	auto txt = fps.AddComponent<TextRenderer>();
	txt->pFont = new Font(L"Arial");
	txt->pBrush = new ColorBrush(Color::Red);
	txt->text = L"FPS";
	txt->rectPosF = new RectanglePosF(0.0f, 0.0f, 200.0f, 50.0f);
	fps.AddScript<FpsBehavior>();

	gce::GameManager::Run(param);
	gce::GameManager::Destroy();

	return 0;
}

