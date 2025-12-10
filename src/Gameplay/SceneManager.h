#pragma once
#include <vector>
#include <unordered_map>
#include "Tags.h"

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
	std::unordered_map<SceneType, std::vector<gce::GameObject*> > m_SceneObjectsList;

	InventoryManager* m_pInventoryManager = nullptr;

	gce::D12PipelineObject* m_pPso = nullptr;
public:
	SceneManager() {}
	~SceneManager() { delete m_pPso; }
	void InitGamePlayScene(gce::Scene& scene1);
	void Init();

	gce::GameObject* GetFirstGameObject(GlobalTag tag1, SecondaryTag tag2 = SecondaryTag::None); // return the first GameObject with the 2 two tags
	std::vector<gce::GameObject*> GetAllGameObjects(GlobalTag tag1, SecondaryTag tag2); // return all GameObjects with the 2 two tags
	std::vector<gce::GameObject*> GetAllGameObjects(GlobalTag tag1); // return all GameObjects with the first Tag

	InventoryManager* GetInventoryManager() { return m_pInventoryManager; }

	gce::D12PipelineObject* GetPSO() { return m_pPso; }
};

