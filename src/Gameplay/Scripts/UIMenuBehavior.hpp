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
std::wstring titleTxt = L"Title Game :3";

EntityWrapper* pButtonPlay = nullptr;
EntityWrapper* 


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
	pTitle->AddTextRenderer(titleTxt, { 860, 400, 0, 0 }, gce::Color::Magenta);

	pButtonPlay = &EntityWrapper::Create();
	pButtonPlay->AddUIButton({ 960, 540 }, { 0, 0 }, { 300, 200 }, "res/Texture/jaune.jpg")->AddListener(OnTriggerButtonPlay);
}

void Update()
{
	bool display = pUIManager->IsSceneType(SceneType::MenuScene);

	pTitle->SetActive(display);
	pButtonPlay->SetActive(display);

	if (display == true)
	{
		pTitle->GetComponent<TextRenderer>()->text = titleTxt;
	}
}

END_SCRIPT
