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
#include "Scripts/UIManagerBehavior.hpp"
#include "Scripts/SceneManagerBehavior.hpp"

#include "Utils.h"
#include <ranges>

void SceneManager::InitGamePlay(gce::Scene& scene)
{
	ArenaCamera ac;
	ac.Create();
	ac.SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);
	ac.SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);

	gce::LightManager::SetLightsProperties(8.0f, 100.0f, 2.0f, 32.0f, 1.f);
	gce::LightData directionalLight = gce::LightManager::CreateDirectionalLight(gce::Vector3f32(0.0f, -1.f, 0.f), gce::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 3.0f, 3.0f);
	gce::LightManager::AddLight(directionalLight);

	Player player;
	player.Create();
	ac.GetScript<CameraBehavior>()->SetGameObjectTagsToFollow(PrimaryTag::TPlayer, SecondaryTag::None);

	LinkObjectToScene(player.GetGameObject(), SceneType::GamePlayScene);

	{
		EntityWrapper& eMogwaiBroken = EntityWrapper::Create();
		eMogwaiBroken.SetProperties("Mogwai", PrimaryTag::TEnemy, SecondaryTag::TMogwai, { 0.5, 2, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
		eMogwaiBroken.AddMeshRenderer(gce::GeometryFactory::LoadGeometry("res/Assets/mogwai_lowcost/mogwai_lowcost.obj"), "res/Assets/mogwai_lowcost/mogwai_lowcost_base_color.png");
		eMogwaiBroken.AddPhysics(10, 1, 0);
		eMogwaiBroken.AddComponent<BoxCollider>();

		LinkObjectToScene(&eMogwaiBroken, SceneType::GamePlayScene);
	}

	const auto& inventory = m_pInventoryManager->GetCurrentInventory();

	for (gce::GameObject* go : inventory)
	{
		ac.GetGameObject()->AddChild(*go);
	}

	EntityWrapper& floor = EntityWrapper::Create();
	floor.SetProperties("Floor", PrimaryTag::TGround, SecondaryTag::None, { 0, -2, 0 }, { 0, 0, 0 }, { 50, 1, 50 });
	floor.AddMeshRenderer(SHAPES.CUBE, "");
	floor.AddComponent<BoxCollider>();

	LinkObjectToScene(&floor, SceneType::GamePlayScene);

	EntityWrapper& UI = EntityWrapper::Create();
	UI.AddScript<UIManagerBehavior>();

	LinkObjectToScene(&UI, SceneType::GamePlayScene);

	//EntityWrapper& button = EntityWrapper::Create();
	//UIButton* comp = button.AddComponent<UIButton>();
	//BitMapBrush* buttonBrush = new BitMapBrush{ "res/Texture/temple_normal_map.png" }; // Image
	//comp->pBitMapBrush = buttonBrush;

	//button.SetProperties("Button", Tag1::TMiscellaneous, Tag2::None, { 960, 540, 0 }, { 0, 0, 0 }, { 500, 250, 1 });

	//Clear the Inventory Tmp Objects because GameObjects are pushed back
	m_pInventoryManager->UnInitTmp();
	m_pInventoryManager->InitStates();

	for (gce::GameObject* go : ImportBlenderScene(L"scene_base.json"))
	{
		LinkObjectToScene(go, SceneType::GamePlayScene);
	}
}

void SceneManager::Init()
{
	gce::Scene& scene = gce::Scene::Create();

	m_pPso = new gce::D12PipelineObject(
		gce::SHADERS.VERTEX,
		gce::SHADERS.PIXEL,
		gce::SHADERS.HULL,
		gce::SHADERS.DOMAIN_,
		gce::SHADERS.ROOT_SIGNATURE
	);

	m_pInventoryManager = new InventoryManager();
	m_pInventoryManager->InitAll();

	m_pEmpty = &EntityWrapper::Create();
	m_pEmpty->SetProperties("SceneManager Object", PrimaryTag::TMiscellaneous, SecondaryTag::None);
	m_pEmpty->AddScript<SceneManagerBehavior>();

	InitGamePlay(scene);
}

void SceneManager::ChangeScene(SceneType newType)
{
	if (m_currentSceneType == newType)
		return;

	for (gce::GameObject* go : m_SceneObjectsList[m_currentSceneType])
	{
		/*auto& allComponents = go->GetAllComponents();

		for (auto it = allComponents.begin(); it != allComponents.end(); ++it)
		{
			gce::Component::TypeEnum type = it->first;

			go->RemoveComponent<type>();
		}*/

		go->Destroy();
	}
	

	for (gce::GameObject* go : m_SceneObjectsList[newType])
	{
		go->SetActive(true);
	}

	m_currentSceneType = newType;
}

void SceneManager::LinkObjectToScene(gce::GameObject* obj, SceneType scene)
{
	for (gce::GameObject* go : m_SceneObjectsList[scene])
	{
		if (go == obj)
		{
			std::cerr << "Obj already linked to a scene !\n";
			return;
		}
	}

	m_SceneObjectsList[scene].push_back(obj);
}

gce::GameObject* SceneManager::GetFirstGameObject(PrimaryTag tag1, SecondaryTag tag2)
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

std::vector<gce::GameObject*> SceneManager::GetAllGameObjects(PrimaryTag tag1, SecondaryTag tag2)
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

std::vector<gce::GameObject*> SceneManager::GetAllGameObjects(PrimaryTag tag1)
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

