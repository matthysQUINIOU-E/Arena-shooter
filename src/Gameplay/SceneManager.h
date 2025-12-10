#pragma once
#include <vector>
#include <unordered_map>
#include "Tags.h"

class Player;
class ArenaCamera;
class EntityWrapper;
class InventoryManager;
class UIManager;

namespace gce
{
	class D12PipelineObject;
}

enum SceneType
{
	MenuScene,
	GamePlayScene,
	PauseScene,
	GameOverScene,
	ArsenalScene,
	Options,
	Count
};

namespace gce {
	class Scene;
	class GameObject;
}

class SceneManager
{
	std::vector<gce::GameObject*> m_Map; // All the objects imported by the json
	EntityWrapper* m_pEmpty = nullptr; // Need to handle the SceneManager Script
	UIManager* m_pUIManager = nullptr; // Handle all the 2D objects

	Player* m_pPlayer = nullptr;
	ArenaCamera* m_pArenaCam = nullptr;

	std::unordered_map<SceneType, std::vector<gce::GameObject*>> m_SceneObjectsList;
	SceneType m_currentSceneType = SceneType::GamePlayScene;

	InventoryManager* m_pInventoryManager = nullptr;

	gce::D12PipelineObject* m_pPso = nullptr;

public:
	SceneManager() = default;
	~SceneManager() { delete m_pPso; }

	void Init();
	void InitGamePlay();
	void UnInitGamePlay();
	bool m_IsGamePlayInit = false;

	void ChangeScene(SceneType newType);
	void LinkObjectToScene(gce::GameObject* obj, SceneType scene);

	gce::GameObject* GetFirstGameObject(PrimaryTag tag1, SecondaryTag tag2); // return the first GameObject with the 2 two tags
	gce::GameObject* GetFirstGameObject(PrimaryTag tag1); // return the first GameObject with the first tag
	std::vector<gce::GameObject*> GetAllGameObjects(PrimaryTag tag1, SecondaryTag tag2); // return all GameObjects with the 2 two tags
	std::vector<gce::GameObject*> GetAllGameObjects(PrimaryTag tag1); // return all GameObjects with the first Tag

	InventoryManager* GetInventoryManager() { return m_pInventoryManager; }
	gce::D12PipelineObject* GetPSO() { return m_pPso; }
	gce::GameObject* GetCameraObject();
	const SceneType& GetSceneType() const { return m_currentSceneType; }
	UIManager* GetUIManager() { return m_pUIManager; }
};

