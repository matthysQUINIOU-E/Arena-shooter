#pragma once
#include <vector>
#include <unordered_map>

enum  Scene
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
	class Camera;
	class WindowParam;
}
class SceneManager
{ 
	std::unordered_map<int, std::vector<gce::GameObject*> > m_SceneObjectsList;

	gce::WindowParam* m_pParam;
	gce::Camera* m_pCamera;
public:
	SceneManager();
	void CreateWindowParam();
	void InitScene();
	void InitGamePlayScene(gce::Scene& scene1);
	void InitCamera();
	void Init();
};

