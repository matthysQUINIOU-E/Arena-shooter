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

void SceneManager::InitGamePlay()
{
	if (m_IsGamePlayInit == true)
		return;
	
	m_IsGamePlayInit = true;

	for (gce::GameObject* go : m_Map)
	{
		go->SetActive(true);
	}

	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new Player();
		m_pPlayer->Create();

		gce::GameObject* pGameObject = m_pPlayer->GetGameObject();

		pGameObject->AddChild(*m_pArenaCam->GetGameObject());
		LinkObjectToScene(pGameObject, SceneType::GamePlayScene);
	}

	m_pInventoryManager->InitAll();

	float camOffsetY = m_pPlayer->GetGameObject()->transform.GetWorldScale().y * 0.5f;

	m_pArenaCam->GetGameObject()->transform.SetLocalPosition({ 0, 0, 0 });

	//EntityWrapper& button = EntityWrapper::Create();
	//UIButton* comp = button.AddComponent<UIButton>();
	//BitMapBrush* buttonBrush = new BitMapBrush{ "res/Texture/temple_normal_map.png" }; // Image
	//comp->pBitMapBrush = buttonBrush;

	//button.SetProperties("Button", Tag1::TMiscellaneous, Tag2::None, { 960, 540, 0 }, { 0, 0, 0 }, { 500, 250, 1 });

	//Clear the Inventory Tmp Objects because GameObjects are pushed back

	m_pInventoryManager->UnInitTmp();
	m_pInventoryManager->InitStates();
}

void SceneManager::UnInitGamePlay()
{
	m_IsGamePlayInit = false;

	for (gce::GameObject* go : m_pInventoryManager->GetCurrentInventory())
	{
		m_pArenaCam->GetGameObject()->RemoveChild(*go);
	}

	m_pInventoryManager->ResetAll();

	for (gce::GameObject* go : m_SceneObjectsList[SceneType::GamePlayScene])
	{
		go->Destroy();
	}

	m_SceneObjectsList[SceneType::GamePlayScene].clear();

	delete m_pPlayer;
	m_pPlayer = nullptr;

	for (gce::GameObject* go : m_Map)
	{
		go->SetActive(false);
	}

	Quaternion reset = { 0, 0, 0, 1 };

	m_pArenaCam->GetGameObject()->transform.SetLocalPosition({ 0, 0, 0 });
	m_pArenaCam->GetGameObject()->transform.SetWorldRotation(reset);
}

void SceneManager::Init()
{
	m_pPso = new gce::D12PipelineObject(
		gce::SHADERS.VERTEX,
		gce::SHADERS.PIXEL,
		gce::SHADERS.HULL,
		gce::SHADERS.DOMAIN_,
		gce::SHADERS.ROOT_SIGNATURE
	);

	gce::Scene& scene = gce::Scene::Create();

	gce::LightManager::SetLightsProperties(8.0f, 100.0f, 2.0f, 32.0f, 1.f);
	gce::LightData directionalLight = gce::LightManager::CreateDirectionalLight(gce::Vector3f32(0.0f, -1.f, 0.f), gce::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 3.0f, 3.0f);
	gce::LightManager::AddLight(directionalLight);

	m_pArenaCam = new ArenaCamera();
	m_pArenaCam->Create();
	m_pArenaCam->SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);

	for (gce::GameObject* go : ImportBlenderScene(L"scene_base.json"))
	{
		go->SetTag1(PrimaryTag::TMapObject);

		m_Map.push_back(go);
	}

	m_pInventoryManager = new InventoryManager();

	m_pEmpty = &EntityWrapper::Create();
	m_pEmpty->SetProperties("SceneManager Object", PrimaryTag::TMiscellaneous, SecondaryTag::None);
	m_pEmpty->AddScript<SceneManagerBehavior>();
	m_pEmpty->AddScript<UIManagerBehavior>();

	InitGamePlay();
}

void SceneManager::ChangeScene(SceneType newType)
{
	if (m_currentSceneType == newType)
		return;

	switch (m_currentSceneType)
	{
	case SceneType::GamePlayScene:
		UnInitGamePlay();
		break;
	default:
		for (gce::GameObject* go : m_SceneObjectsList[m_currentSceneType])
		{
			go->SetActive(false);
		}
	}

	switch (newType)
	{
	case SceneType::GamePlayScene:
		m_IsGamePlayInit = false;
		InitGamePlay();
		break;
	default:
		for (gce::GameObject* go : m_SceneObjectsList[newType])
		{
			go->SetActive(true);
		}
	}

	m_currentSceneType = newType;
}

void SceneManager::LinkObjectToScene(gce::GameObject* obj, SceneType scene)
{
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

gce::GameObject* SceneManager::GetCameraObject()
{
	return m_pArenaCam->GetGameObject();
}

