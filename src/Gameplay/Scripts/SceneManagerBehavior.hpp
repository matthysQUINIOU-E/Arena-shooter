#pragma once
#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "GameManager.h"
#include "../SceneManager.h"
#include "../Prefabs/InventoryManager.h"
#include "../Tags.h"
#include "KeyBinds.h"

using namespace gce;

DECLARE_SCRIPT(SceneManagerBehavior, ScriptFlag::Start | ScriptFlag::Update)

SceneManager* pSceneManager = nullptr;

void Start()
{
	pSceneManager = &GameManager::GetSceneManager();
}

void Update()
{
	if (GetKeyDown(KeyBinds::GetKeyBind(KeyAction::Crash)))
	{
		abort();
		return;
	}

	if (pSceneManager == nullptr)
		return;

	if (GetKeyDown(Keyboard::H)) // DESTROY ALLL JFHOIDHFOIDHFOIDFHOIUFHO
	{
		pSceneManager->ChangeScene(SceneType::MenuScene);
	}
	if (GetKeyDown(Keyboard::J)) // DESTROY ALLL JFHOIDHFOIDHFOIDFHOIUFHO
	{
		pSceneManager->ChangeScene(SceneType::GamePlayScene);
	}
}

END_SCRIPT