#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "Prefabs/EntityWrapper.h"

#include "Scripts/AmmoManagerBehavior.hpp"
#include "Scripts/GunBehavior.hpp"
#include "GameManager.h"
#include "SceneManager.h"
#include "Prefabs/InventoryManager.h"

using namespace gce;

DECLARE_SCRIPT(UIManagerBehavior, ScriptFlag::Start | ScriptFlag::Update)

//Members
EntityWrapper* pFpsUI = nullptr;
std::wstring fpsTxt;
float mRefreshProgress = 0.f;

EntityWrapper* pAmmoUI = nullptr;
std::wstring ammoTxt;

//Functions
void UpdateFPSTxt()
{
	if (mRefreshProgress < 0)
	{
		mRefreshProgress = 0.5f;

		fpsTxt = L"FPS : " + std::to_wstring((int)GameManager::FPS());
		pFpsUI->GetComponent<TextRenderer>()->text = fpsTxt;
	}
	else
	{
		mRefreshProgress -= GameManager::DeltaTime();
	}
}

void UpdateAmmoTxt()
{
	gce::GameObject* pObj = GameManager::GetSceneManager().GetInventoryManager()->GetCurrentEquipedObject();

	if (pObj->IsTag1(Tag1::TWeapon))
	{
		AmmoManagerBehavior* script = pObj->GetScript<AmmoManagerBehavior>();

		ammoTxt = L"Ammos : " + std::to_wstring(script->ammos) + L"/" + std::to_wstring(script->maxAmmos);
	}
	else
	{
		ammoTxt = L"To Do";
	}

	pAmmoUI->GetComponent<TextRenderer>()->text = ammoTxt;
}

void Start()
{
	pFpsUI = &EntityWrapper::Create();
	pFpsUI->AddTextRenderer(L"Fps", { 0, 0, 300, 300 }, gce::Color::Black);

	pAmmoUI = &EntityWrapper::Create();

	gce::Vector3f32 ammoPos = { 1480, 900, 0.f };
	pAmmoUI->AddTextRenderer(L"Ammos", { ammoPos.x, ammoPos.y, 0, 0 }, gce::Color::Red);
}

void Update()
{
	UpdateFPSTxt();
	UpdateAmmoTxt();
}

END_SCRIPT
