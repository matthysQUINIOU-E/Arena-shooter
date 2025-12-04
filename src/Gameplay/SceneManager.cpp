#include <Console.h>
#include "SceneManager.h"
#include <windows.h>
#include <Render.h>
#include <Engine.h>
#include "Prefabs/ArenaCamera.h"
#include "Prefabs/Player.h"

#include "Scripts/CameraBehavior.hpp"
#include "Scripts/FpsBehavior.hpp"
#include "Utils.h"


void SceneManager::InitGamePlayScene(gce::Scene& scene)
{
	ArenaCamera ac;
	ac.Create(scene);
	ac.SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);
	ac.SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);

	Player player;
	player.Create(scene);
	player.TestMusket(scene);

	gce::LightData directionalLight = gce::LightManager::CreateDirectionalLight(gce::Vector3f32(0.0f, 0.0f, 1.0f), gce::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
	gce::LightManager::AddLight(directionalLight);

	ac.GetScript<CameraBehavior>()->SetGameObjectToFollow(player.GetGameObject());

	ac.GetGameObject()->AddChild(*player.GetWeapon("musket"));

	ImportBlenderScene(L"scene_base.json");

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
	InitGamePlayScene(scene);
}

