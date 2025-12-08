#include <Console.h>
#include "SceneManager.h"
#include <windows.h>
#include <Render.h>
#include <Engine.h>

#include "Prefabs/InventoryManager.h"
#include "Prefabs/UIManager.h"

#include "Prefabs/ArenaCamera.h"
#include "Prefabs/Player.h"
#include "Prefabs/EntityWrapper.h"
#include "Scripts/CameraBehavior.hpp"
#include "Scripts/PlayerBehavior.hpp"
#include "Scripts/UIManagerBehavior.hpp"

#include "Utils.h"
#include <ranges>

void SceneManager::InitGamePlayScene(gce::Scene& scene)
{
	ArenaCamera ac;
	ac.Create();
	ac.SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);

	gce::LightManager::SetLightsProperties(8.0f, 100.0f, 2.0f, 32.0f, 1.f);
	gce::LightData directionalLight = gce::LightManager::CreateDirectionalLight(gce::Vector3f32(0.0f, -1.f, 0.f), gce::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 3.0f, 3.0f);
	gce::LightManager::AddLight(directionalLight);

	Player player;
	player.Create();
	ac.GetScript<CameraBehavior>()->SetGameObjectToFollow(player.GetGameObject());

	{
		EntityWrapper& eMogwaiBroken = EntityWrapper::Create();
		eMogwaiBroken.SetProperties("Mogwai", GlobalTag::TEnemy, SecondaryTag::TMogwai, { 0.5, 2, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
		eMogwaiBroken.AddMeshRenderer(gce::GeometryFactory::LoadGeometry("res/Assets/mogwai_lowcost/mogwai_lowcost.obj"), "res/Assets/mogwai_lowcost/mogwai_lowcost_base_color.png");
		eMogwaiBroken.AddPhysics(10, 1, 0);
		eMogwaiBroken.AddComponent<BoxCollider>();
	}

	const auto& inventory = m_pInventoryManager->GetCurrentInventory();

	for (gce::GameObject* go : inventory)
	{
		ac.GetGameObject()->AddChild(*go);
	}

	EntityWrapper& floor = EntityWrapper::Create();
	floor.SetProperties("Floor", GlobalTag::TGround, SecondaryTag::None, { 0, -2, 0 }, { 0, 0, 0 }, { 50, 1, 50 });
	floor.AddMeshRenderer(SHAPES.CUBE, "");
	floor.AddComponent<BoxCollider>();

	EntityWrapper& UI = EntityWrapper::Create();
	UI.AddScript<UIManagerBehavior>();

	//EntityWrapper& button = EntityWrapper::Create();
	//UIButton* comp = button.AddComponent<UIButton>();
	//BitMapBrush* buttonBrush = new BitMapBrush{ "res/Texture/temple_normal_map.png" }; // Image
	//comp->pBitMapBrush = buttonBrush;

	//button.SetProperties("Button", Tag1::TMiscellaneous, Tag2::None, { 960, 540, 0 }, { 0, 0, 0 }, { 500, 250, 1 });

	//Clear the Inventory Tmp Objects because GameObjects are pushed back


	m_pInventoryManager->UnInitTmp();
	m_pInventoryManager->InitStates();
}

void SceneManager::Init()
{
	gce::Scene& scene = gce::Scene::Create();

	pPso = new gce::D12PipelineObject(
		gce::SHADERS.VERTEX,
		gce::SHADERS.PIXEL,
		gce::SHADERS.HULL,
		gce::SHADERS.DOMAIN_,
		gce::SHADERS.ROOT_SIGNATURE
	);

	m_pInventoryManager = new InventoryManager();
	m_pInventoryManager->InitAll();

	InitGamePlayScene(scene);
}

gce::GameObject* SceneManager::GetFirstGameObject(GlobalTag tag1, SecondaryTag tag2)
{
	auto& gameObjects = GameManager::GetScene().m_gameObjects;

	for (GameObject* pGameObject : gameObjects | std::views::values)
	{
		if (pGameObject->IsTag1(tag1) && pGameObject->IsTag2(tag2))
		{
			return pGameObject;
		}
	}

	return nullptr;
}

std::vector<gce::GameObject*> SceneManager::GetAllGameObjects(GlobalTag tag1, SecondaryTag tag2)
{
	auto& gameObjects = GameManager::GetScene().m_gameObjects;

	std::vector<gce::GameObject*> finalTab;

	for (GameObject* pGameObject : gameObjects | std::views::values)
	{
		if (pGameObject->IsTag1(tag1) && pGameObject->IsTag2(tag2))
		{
			finalTab.push_back(pGameObject);
		}
	}

	return finalTab;
}

std::vector<gce::GameObject*> SceneManager::GetAllGameObjects(GlobalTag tag1)
{
	auto& gameObjects = GameManager::GetScene().m_gameObjects;

	std::vector<gce::GameObject*> finalTab;

	for (GameObject* pGameObject : gameObjects | std::views::values)
	{
		if (pGameObject->IsTag1(tag1))
		{
			finalTab.push_back(pGameObject);
		}
	}

	return finalTab;
}

