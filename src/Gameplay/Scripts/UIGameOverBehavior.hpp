#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "Prefabs/EntityWrapper.h"

#include "GameManager.h"
#include "SceneManager.h"
#include "Prefabs/InventoryManager.h"
#include "Prefabs/UIManager.h"

using namespace gce;

DECLARE_SCRIPT(UIGameOverBehavior, ScriptFlag::Start | ScriptFlag::Update)

UIManager* pUIManager = nullptr;

//Members
EntityWrapper* pTitle = nullptr;

EntityWrapper* pButtonPlay = nullptr;
EntityWrapper* pPlay = nullptr;

EntityWrapper* pButtonMenu = nullptr;
EntityWrapper* pMenu = nullptr;

std::wstring NOTHING = L"";

//Functions

static void OnTriggerButtonPlay()
{
	GameManager::GetSceneManager().ChangeScene(SceneType::GamePlayScene);
}

static void OnTriggerButtonMenu()
{
	GameManager::GetSceneManager().ChangeScene(SceneType::MenuScene);
}

void Start()
{
	pUIManager = GameManager::GetSceneManager().GetUIManager();

	pTitle = &EntityWrapper::Create();
	pTitle->AddStaticTextRenderer(L"Game Over >W<", { 800, 350, 0, 0 }, gce::Color::Magenta);

	//PLAY
	pButtonPlay = &EntityWrapper::Create();
	pButtonPlay->AddUIButton({ 960, 540 }, { 0, 0 }, { 300, 150 }, "res/Texture/jaune.jpg")->AddListener(OnTriggerButtonPlay);

	pPlay = &EntityWrapper::Create();
	pPlay->AddStaticTextRenderer(L"Try Again", { 850, 500,0, 0 }, gce::Color::Black);

	//MENU
	pButtonMenu = &EntityWrapper::Create();
	pButtonMenu->AddUIButton({ 960, 750 }, { 0, 0 }, { 300, 150 }, "res/Texture/jaune.jpg")->AddListener(OnTriggerButtonMenu);

	pMenu = &EntityWrapper::Create();
	pMenu->AddStaticTextRenderer(L"Give Up :C", { 840, 710, 0, 0 }, gce::Color::Black);
}

void Update()
{
	bool display = pUIManager->IsSceneType(SceneType::GameOverScene);

	if (display == true)
		ShowMouseCursor();

	pTitle->SetActive(display);

	pButtonPlay->SetActive(display);
	pPlay->SetActive(display);

	pButtonMenu->SetActive(display);
	pMenu->SetActive(display);
}

END_SCRIPT
