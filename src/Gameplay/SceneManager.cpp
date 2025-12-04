#include <Console.h>
#include "SceneManager.h"
#include <windows.h>
#include <Render.h>
#include <Engine.h>
#include "Prefabs/ArenaCamera.h"
#include "Scripts/PlayerBehavior.hpp"
#include "Scripts/CameraBehavior.hpp"
#include "Scripts/FpsBehavior.hpp"
#include "Utils.h"


void SceneManager::InitGamePlayScene(gce::Scene& scene)
{
	ArenaCamera ac;
	ac.Create(scene);
	ac.SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);


	gce::D12PipelineObject* pso = new gce::D12PipelineObject( // TODO ::change
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

	gce::GameObject& player = gce::GameObject::Create(scene);
	player.SetName("Player");
	player.transform.SetWorldScale({ 1, 2, 1 });
	gce::PhysicComponent* pPhysic = player.AddComponent<gce::PhysicComponent>();
	gce::MeshRenderer* pPlayerMesh = player.AddComponent<gce::MeshRenderer>();
	pPlayerMesh->pGeometry = gce::SHAPES.CUBE;

	pPlayerMesh->pMaterial->albedoTextureID = pWhiteTexture->GetTextureID();
	pPlayerMesh->pMaterial->useTextureAlbedo = 1;
	pPlayerMesh->pMaterial->subsurface = 1;
	pPlayerMesh->pPso = pso;

	gce::BoxCollider* bCollider = player.AddComponent<gce::BoxCollider>();

	PlayerBehavior* pScript = player.AddScript<PlayerBehavior>();

	ac.GetScript<CameraBehavior>()->SetGameObjectToFollow(&player);

	Texture* pArmsBaseColor = new Texture("res/Assets/zhu_rong_arms/Arms_Base_Color.png");

	gce::GameObject& playerArms = gce::GameObject::Create(scene);
	playerArms.SetName("Player_Arms");
	playerArms.transform.SetLocalPosition(ac.GetGameObject()->transform.GetWorldPosition());
	playerArms.transform.SetWorldScale({ 3, 3, 3 });
	gce::MeshRenderer* pArmsMesh = playerArms.AddComponent<gce::MeshRenderer>();
	pArmsMesh->pGeometry = gce::GeometryFactory::LoadGeometry("res/Assets/zhu_rong_arms/Arms.obj");
	pArmsMesh->pMaterial->albedoTextureID = pArmsBaseColor->GetTextureID();
	pArmsMesh->pMaterial->useTextureAlbedo = 1;
	pArmsMesh->pMaterial->subsurface = 1;
	pArmsMesh->pPso = pso;

	ac.GetGameObject()->AddChild(playerArms);

	gce::GameObject& floor = gce::GameObject::Create(scene);
	floor.transform.SetLocalScale({ 50, 1, 50 });
	floor.transform.SetWorldPosition({ 0, -2, 0 });
	floor.SetName("Floor");
	gce::MeshRenderer* pFloorMesh = floor.AddComponent<gce::MeshRenderer>();
	pFloorMesh->pGeometry = gce::SHAPES.CUBE;

	pFloorMesh->pMaterial->albedoTextureID = pWhiteTexture->GetTextureID();
	pFloorMesh->pMaterial->useTextureAlbedo = 1;
	pFloorMesh->pMaterial->subsurface = 1;
	pFloorMesh->pPso = pso;

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

