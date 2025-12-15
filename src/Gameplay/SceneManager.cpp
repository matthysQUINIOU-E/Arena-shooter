#include <Console.h>
#include "SceneManager.h"
#include <windows.h>
#include <Render.h>
#include <Engine.h>

#include "Prefabs/InventoryManager.h"

#include "Prefabs/ArenaCamera.h"
#include "Prefabs/Player.h"

#include "Prefabs/EntityWrapper.h"
#include "Prefabs/UIManager.h"

#include "Scripts/CameraBehavior.hpp"

#include "Scripts/SceneManagerBehavior.hpp"

#include "Scripts/AgentBehavior.hpp"
#include "Scripts/WaveManagerBehavior.hpp"

#include "Prefabs/BonusManager.h"
#include "Prefabs/BulletPool.h"

#include "Utils.h"
#include "Agent.h"
#include <ranges>

void SceneManager::InitGamePlay()
{
	if (m_IsGamePlayInit == true) // bonobo
		return;
	
	m_IsGamePlayInit = true;

	for (gce::GameObject* go : m_Map)
	{
		go->SetActive(true);
	}

	m_pPlayer = new Player();
	m_pPlayer->Create();

	gce::GameObject* pGameObject = m_pPlayer->GetGameObject();
	
	pGameObject->AddChild(*GetCameraObject());

	LinkObjectToScene(pGameObject, SceneType::GamePlayScene);

	float camOffsetY = m_pPlayer->GetGameObject()->transform.GetWorldScale().y * 0.5f;
	GetCameraObject()->transform.SetLocalPosition({ 0, camOffsetY, 0 });

	m_pInventoryManager->InitAll();
	m_pInventoryManager->InitInventoryState();

	EntityWrapper& entityWrapper = EntityWrapper::Create();
	entityWrapper.AddScript<WaveManagerBehavior>();
	LinkObjectToScene(&entityWrapper, SceneType::GamePlayScene);

	BonusManager::CreateNem({ 5, 2, 0 });
	BonusManager::CreateRiceBowl({ 5, 2, -1 });
	BonusManager::CreateNoodles({ 5, 2, 1 });
}

void SceneManager::UnInitGamePlay()
{
	m_IsGamePlayInit = false;

	for (gce::GameObject* go : m_pInventoryManager->GetCurrentInventory())
	{
		m_pArenaCam->GetGameObject()->RemoveChild(*go);
	}

	m_pInventoryManager->ResetAll();

	m_pPlayer->GetGameObject()->RemoveChild(*GetCameraObject());

	for (gce::GameObject* go : m_SceneObjectsList[SceneType::GamePlayScene])
	{
		if (go->HasTags({ Tag::TMapObject }) == false)
			go->Destroy();
	}
	// sigma
	m_SceneObjectsList[SceneType::GamePlayScene].clear();

	delete m_pPlayer;
	m_pPlayer = nullptr;

	BulletPool::DesactivateAllBullets();

	for (gce::GameObject* go : m_Map)
	{
		go->SetActive(false);
	}
}

void SceneManager::Init()
{

	//Keys
	KeyBinds::InitDefaultKeyBinds();

	//PSO
	m_pPso = new gce::D12PipelineObject(
		gce::SHADERS.VERTEX,
		gce::SHADERS.PIXEL,
		gce::SHADERS.HULL,
		gce::SHADERS.DOMAIN_,
		gce::SHADERS.ROOT_SIGNATURE
	);

	gce::Scene& scene = gce::Scene::Create();
	
	// LIGHT
	gce::LightManager::SetLightsProperties(8.0f, 100.0f, 2.0f, 32.0f, 1.f);
	gce::LightData directionalLight = gce::LightManager::CreateDirectionalLight(gce::Vector3f32(0.0f, -1.f, 0.f), gce::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 3.0f, 3.0f);
	gce::LightManager::AddLight(directionalLight);

	//CAM
	m_pArenaCam = new ArenaCamera();
	m_pArenaCam->Create();
	m_pArenaCam->SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);

	//MAP
	for (gce::GameObject* go : ImportBlenderScene(L"scene_base.json"))
	{
		go->AddTags({ Tag::TMapObject });
		go->SetActive(false);
		m_Map.push_back(go);
	}

	//INVENTORY
	m_pInventoryManager = new InventoryManager();

	//BulletPool
	BulletPool::Init();

	//Scene Manager Behavior
	m_pEmpty = &EntityWrapper::Create();
	m_pEmpty->SetProperties("SceneManager Object", { Tag::TMiscellaneous });
	m_pEmpty->AddScript<SceneManagerBehavior>();

	//UI
	m_pUIManager = new UIManager();
	m_pUIManager->Init();

	ChangeScene(SceneType::GamePlayScene);
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

gce::GameObject* SceneManager::GetFirstGameObject(std::vector<Tag> tags)
{
	auto& gameObjects = GameManager::GetScene().m_gameObjects;

	for (GameObject* pGameObject : gameObjects | std::views::values)
	{
		if (pGameObject->HasTags(tags))
			return pGameObject;
	}

	return nullptr;
}

std::vector<gce::GameObject*> SceneManager::GetAllGameObjects(std::vector<Tag> tags)
{
	auto& gameObjects = GameManager::GetScene().m_gameObjects;

	std::vector<gce::GameObject*> finalTab;

	for (GameObject* pGameObject : gameObjects | std::views::values)
	{
		if (pGameObject->HasTags(tags))
			finalTab.push_back(pGameObject);
	}

	return finalTab;
}

gce::GameObject* SceneManager::GetCameraObject()
{
	return m_pArenaCam->GetGameObject();
}

void SceneManager::SetFullScreenMode(bool state)
{
	if (m_fullScreen == state)
		return;

	m_fullScreen = state;

	auto win = GameManager::GetWindow();

	if (m_fullScreen == false)
	{
		win->SetFullScreen(gce::FullScreenMode::WINDOWED);
	}
	else
	{
		win->SetFullScreen(gce::FullScreenMode::BORDERLESS);
	}
}

void SceneManager::ToggleFullScreenMode()
{
	m_fullScreen = !m_fullScreen;

	auto win = GameManager::GetWindow();

	if (m_fullScreen == false)
	{
		win->SetFullScreen(gce::FullScreenMode::WINDOWED);
	}
	else
	{
		win->SetFullScreen(gce::FullScreenMode::BORDERLESS);
	}
}


