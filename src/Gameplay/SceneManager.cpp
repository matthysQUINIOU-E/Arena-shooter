#include <Console.h>
#include "SceneManager.h"
#include <windows.h>
#include <Render.h>
#include <Engine.h>
#include "Prefabs/ArenaCamera.h"
#include "Prefabs/Player.h"

#include "Scripts/CameraBehavior.hpp"
#include "Scripts/FpsBehavior.hpp"
#include "Utils.h"


void SceneManager::InitGamePlayScene(gce::Scene& scene)
{
	ArenaCamera ac;
	ac.Create(scene);
	ac.SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);


	pPso = new gce::D12PipelineObject( // TODO ::change
		gce::SHADERS.VERTEX,
		gce::SHADERS.PIXEL,
		gce::SHADERS.HULL,
		gce::SHADERS.DOMAIN_,
		gce::SHADERS.ROOT_SIGNATURE
	);

	gce::Texture* pWhiteTexture = new gce::Texture("res/Assets/white.png");

	gce::LightManager::SetLightsProperties(8.0f, 100.0f, 2.0f, 32.0f, 1.f);
	gce::LightData directionalLight = gce::LightManager::CreateDirectionalLight(gce::Vector3f32(0.0f, 0.0f, 1.0f), gce::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
	gce::LightManager::AddLight(directionalLight);

	Player player;
	player.Create(scene);
	player.TestMusket(scene);

	ac.GetScript<CameraBehavior>()->SetGameObjectToFollow(player.GetGameObject());

	ac.GetGameObject()->AddChild(*player.GetWeapon("musket"));
	
	gce::GameObject& floor = gce::GameObject::Create(scene);
	floor.transform.SetLocalScale({ 50, 1, 50 });
	floor.transform.SetWorldPosition({ 0, -2, 0 });
	floor.SetName("Floor");
	gce::MeshRenderer* pFloorMesh = floor.AddComponent<gce::MeshRenderer>();
	pFloorMesh->pGeometry = gce::SHAPES.CUBE;

	pFloorMesh->pMaterial->albedoTextureID = pWhiteTexture->GetTextureID();
	pFloorMesh->pMaterial->useTextureAlbedo = 1;
	pFloorMesh->pMaterial->subsurface = 1;
	pFloorMesh->pPso = pPso;

	BoxCollider* pFloorBox = floor.AddComponent<BoxCollider>();


	gce::GameObject& fps = gce::GameObject::Create(scene);
	auto txt = fps.AddComponent<TextRenderer>();
	txt->pFont = new Font(L"Arial");
	txt->pBrush = new ColorBrush(Color::Red);
	txt->text = L"FPS";
	txt->rectPosF = new RectanglePosF(0.0f, 0.0f, 200.0f, 50.0f);
	fps.AddScript<FpsBehavior>();
}

void SceneManager::Init()
{
	gce::Scene& scene = gce::Scene::Create();
	InitGamePlayScene(scene);
}

