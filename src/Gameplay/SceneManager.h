#pragma once
#include <vector>
#include <unordered_map>

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

	gce::GameObject* m_pPlayer = nullptr;
public:
	SceneManager() {};
	void InitGamePlayScene(gce::Scene& scene1);
	void Init();
	gce::GameObject* GetPlayer() { return m_pPlayer; }
};

