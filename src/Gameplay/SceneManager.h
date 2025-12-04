#pragma once
#include <vector>
#include <unordered_map>

namespace gce
{
	class D12PipelineObject;
}

enum  SceneType
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
};

