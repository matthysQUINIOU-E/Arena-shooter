#pragma once
#include <vector>
#include <unordered_map>
#include "Tags.h"

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

	gce::D12PipelineObject* pPso = nullptr;
public:
	SceneManager() {};
	void InitGamePlayScene(gce::Scene& scene1);
	void Init();

	gce::D12PipelineObject* GetPSO() { return pPso; }

	gce::GameObject* GetFirstGameObject(Tag1 tag1, Tag2 tag2); // return the first GameObject with the 2 two tags
	std::vector<gce::GameObject*> GetAllGameObjects(Tag1 tag1, Tag2 tag2); // return all GameObjects with the 2 two tags
	std::vector<gce::GameObject*> GetAllGameObjects(Tag1 tag1); // return all GameObjects with the first Tag
};

