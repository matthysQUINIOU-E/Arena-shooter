#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "Prefabs/EntityWrapper.h"

#include "Scripts/WeaponMagazineBehavior.hpp"
#include "Scripts/GunBehavior.hpp"
#include "Scripts/HealthBehavior.hpp"

#include "GameManager.h"
#include "SceneManager.h"
#include "Prefabs/InventoryManager.h"
#include "Prefabs/UIManager.h"

using namespace gce;

DECLARE_SCRIPT(UIMenuBehavior, ScriptFlag::Start | ScriptFlag::Update)

UIManager* pUIManager = nullptr;

//Members
EntityWrapper* pTitle = nullptr;

EntityWrapper* pButtonPlay = nullptr;
EntityWrapper* pPlay = nullptr;

std::wstring NOTHING = L"";

//Functions

static void OnTriggerButtonPlay()
{
	GameManager::GetSceneManager().ChangeScene(SceneType::GamePlayScene);
}

void Start()
{
	pUIManager = GameManager::GetSceneManager().GetUIManager();

	pTitle = &EntityWrapper::Create();
	pTitle->AddStaticTextRenderer(L"The Legend of Zhu Min : God Legacy", {1000, 250, -450, 0}, gce::Color::Magenta);

	//PLAY
	pButtonPlay = &EntityWrapper::Create();
	pButtonPlay->AddUIButton({ 960, 540 }, { 0, 0 }, { 300, 150 }, "res/Texture/jaune.jpg")->AddListener(OnTriggerButtonPlay);

	pPlay = &EntityWrapper::Create();
	pPlay->AddStaticTextRenderer(L"PLAY", {900, 500,0, 0}, gce::Color::Black);
}

void Update()
{
	bool display = pUIManager->IsSceneType(SceneType::MenuScene);

	pTitle->SetActive(display);

	pButtonPlay->SetActive(display);
	pPlay->SetActive(display);
}

END_SCRIPT
