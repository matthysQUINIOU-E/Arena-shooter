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
	ac.SetParams(XM_PIDIV4, 0.001f, 5000.0f, 1000.0f / 800.0f);
	ac.GetGameObject()->transform.SetWorldPosition({ 0.f,100.f,0.f });

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

	gce::GameObject& player = gce::GameObject::Create(scene);
	player.SetName("Player");
	player.transform.SetWorldScale({ 1, 2, 1 });
	player.transform.SetWorldPosition({ 0.f,100.f,0.f });
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
	ImportBlenderScene(L"scene_base.json");

	/*
	gce::GameObject& fps = gce::GameObject::Create(scene);
	auto txt = fps.AddComponent<TextRenderer>();
	txt->pFont = new Font(L"Arial");
	txt->pBrush = new ColorBrush(Color::Red);
	txt->text = L"FPS";
	txt->rectPosF = new RectanglePosF(0.0f, 0.0f, 200.0f, 50.0f);
	fps.AddScript<FpsBehavior>();
	*/
	
	
	//shapeCustom.AddComponent<BoxCollider>();
}

void SceneManager::Init()
{
	gce::Scene& scene = gce::Scene::Create();
	InitGamePlayScene(scene);
}

