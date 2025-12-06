#include <Console.h>
#include "SceneManager.h"
#include <windows.h>
#include <Render.h>
#include <Engine.h>

#include "Prefabs/ArenaCamera.h"
#include "Prefabs/Player.h"
#include "Prefabs/EntityWrapper.h"
#include "Scripts/CameraBehavior.hpp"
#include "Scripts/PlayerBehavior.hpp"
#include "Scripts/GunBehavior.hpp"
#include "Scripts/FpsBehavior.hpp"
#include "Utils.h"
#include <ranges>

void SceneManager::InitGamePlayScene(gce::Scene& scene)
{
	ArenaCamera ac;
	ac.Create();
	ac.SetParams(XM_PIDIV4, 0.001f, 500.0f, 1000.0f / 800.0f);

	gce::LightManager::SetLightsProperties(8.0f, 100.0f, 2.0f, 32.0f, 1.f);
	gce::LightData directionalLight = gce::LightManager::CreateDirectionalLight(gce::Vector3f32(0.0f, 0.0f, 1.0f), gce::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 2.0f, 2.0f);
	gce::LightManager::AddLight(directionalLight);

	Player player;
	player.Create();
	ac.GetScript<CameraBehavior>()->SetGameObjectToFollow(player.GetGameObject());

	{
		EntityWrapper& eMogwaiBroken = EntityWrapper::Create();
		eMogwaiBroken.SetProperties("Mogwai", Tag1::TEnemy, Tag2::TMogwai, { 0.5, 2, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
		eMogwaiBroken.AddMeshRenderer(gce::GeometryFactory::LoadGeometry("res/Assets/mogwai_lowcost/mogwai_lowcost.obj"), "res/Assets/mogwai_lowcost/mogwai_lowcost_base_color.png");
		eMogwaiBroken.AddPhysics(10, 1, 0);
		eMogwaiBroken.AddComponent<BoxCollider>();
	}

	EntityWrapper& musket = EntityWrapper::Create();
	ac.GetGameObject()->AddChild(musket);

	musket.SetProperties("Musket", Tag1::TWeapon, Tag2::TMusket,{ 0, 0, 0 }, { gce::PI, 0, gce::PI }, { 1, 1, 1 });
	musket.transform.SetLocalPosition({ 0.25, -0.1f, 0.5f });

	musket.AddMeshRenderer(gce::GeometryFactory::LoadGeometry("res/Assets/musket/musket.obj"), "res/Assets/musket/musket_base_color.png");

	EntityWrapper& hole = EntityWrapper::Create();
	gce::Vector3f32 holePos = musket.transform.GetWorldPosition();
	holePos.z += 0.5;
	holePos.y += 0.03;
	holePos.x += 0;
	hole.SetChildProperties(musket, "Musket Hole", Tag1::TMiscellaneous, Tag2::None, { 0, 0, 0 }, { 0, 0, 0 }, { 0.05, 0.05, 0.05 });
	hole.transform.SetWorldPosition(holePos);

	musket.AddScript<GunBehavior>();
	player.GetGameObject()->GetScript<PlayerBehavior>()->SetCurrentGun(&musket);

	EntityWrapper& floor = EntityWrapper::Create();
	floor.SetProperties("Floor", Tag1::TGround, Tag2::None, { 0, -2, 0 }, { 0, 0, 0 }, { 50, 1, 50 });
	floor.AddMeshRenderer(SHAPES.CUBE, "");
	floor.AddComponent<BoxCollider>();

	EntityWrapper& fps = EntityWrapper::Create();

	fps.AddTextRenderer(L"FPS", { 0.f, 0.f, 200.f, 50.f }, gce::Color::Red);
	fps.AddScript<FpsBehavior>();
}

void SceneManager::Init()
{
	gce::Scene& scene = gce::Scene::Create();

	pPso = new gce::D12PipelineObject(
		gce::SHADERS.VERTEX,
		gce::SHADERS.PIXEL,
		gce::SHADERS.HULL,
		gce::SHADERS.DOMAIN_,
		gce::SHADERS.ROOT_SIGNATURE
	);

	InitGamePlayScene(scene);
}

gce::GameObject* SceneManager::GetFirstGameObject(Tag1 tag1, Tag2 tag2)
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

std::vector<gce::GameObject*> SceneManager::GetAllGameObjects(Tag1 tag1, Tag2 tag2)
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

std::vector<gce::GameObject*> SceneManager::GetAllGameObjects(Tag1 tag1)
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

