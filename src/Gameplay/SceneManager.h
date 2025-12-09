#pragma once
#include <vector>
#include <unordered_map>
#include "Tags.h"

class EntityWrapper;
class InventoryManager;

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
	EntityWrapper* m_pEmpty = nullptr; // Need to handle the SceneManager Script

	std::unordered_map<SceneType, std::vector<gce::GameObject*> > m_SceneObjectsList;
	SceneType m_currentSceneType = SceneType::GamePlayScene;

	InventoryManager* m_pInventoryManager = nullptr;

	gce::D12PipelineObject* m_pPso = nullptr;

public:
	SceneManager() {}
	~SceneManager() { delete m_pPso; }
	void InitGamePlay(gce::Scene& scene1);
	void Init();

	void ChangeScene(SceneType newType);

	void LinkObjectToScene(gce::GameObject* obj, SceneType scene);

	gce::GameObject* GetFirstGameObject(PrimaryTag tag1, SecondaryTag tag2); // return the first GameObject with the 2 two tags
	std::vector<gce::GameObject*> GetAllGameObjects(PrimaryTag tag1, SecondaryTag tag2); // return all GameObjects with the 2 two tags
	std::vector<gce::GameObject*> GetAllGameObjects(PrimaryTag tag1); // return all GameObjects with the first Tag

	InventoryManager* GetInventoryManager() { return m_pInventoryManager; }

	gce::D12PipelineObject* GetPSO() { return m_pPso; }
};

