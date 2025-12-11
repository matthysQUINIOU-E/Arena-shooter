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
#include "../Prefabs/InventoryManager.h"
#include "../Prefabs/UIManager.h"

#include "../Prefabs/UiBar.h"

using namespace gce;

DECLARE_SCRIPT(UIGameplayBehavior, ScriptFlag::Start | ScriptFlag::Update)

UIManager* pUIManager = nullptr;

//Members
EntityWrapper* pFpsUI = nullptr;
std::wstring fpsTxt;
float mRefreshProgress = 0.f;

EntityWrapper* pAmmoUI = nullptr;
std::wstring ammoTxt;

EntityWrapper* pTotalAmmoUI = nullptr;
std::wstring totalAmmoTxt;

EntityWrapper* pHpUI = nullptr;
std::wstring hpTxt;

EntityWrapper* pCrosshair = nullptr;

UiBar hpBar;

std::wstring NOTHING = L"";


//Functions
void UpdateFpsUI()
{
	if (mRefreshProgress < 0)
	{
		mRefreshProgress = 0.5f;

		fpsTxt = L"FPS : " + std::to_wstring((int)GameManager::FPS());
		pFpsUI->UpdateDynamicText(fpsTxt);
	}
	else
	{
		mRefreshProgress -= GameManager::DeltaTime();
	}
}

void UpdateAmmosUI()
{
	gce::GameObject* pObj = GameManager::GetSceneManager().GetInventoryManager()->GetCurrentEquipedObject();

	if (pObj == nullptr)
		return;

	if (pObj->HasTags({ Tag::TWeapon }))
	{
		WeaponMagazineBehavior* pScript = pObj->GetScript<WeaponMagazineBehavior>();

		if (pScript)
		{
			ammoTxt = L"Ammos : " + std::to_wstring(pScript->ammosLeft) + L"/" + std::to_wstring(pScript->maxCapacity);
		}
		else
		{
			ammoTxt = NOTHING;
		}
	}
	else
	{
		ammoTxt = L"To Do";
	}

	pAmmoUI->UpdateDynamicText(ammoTxt);
}

void UpdateTotalAmmoUI()
{
	InventoryManager* pInventory = GameManager::GetSceneManager().GetInventoryManager();

	gce::GameObject* pObj = pInventory->GetCurrentEquipedObject();

	if (pObj == nullptr)
		return;

	if (pObj->HasTags({ Tag::TWeapon }))
	{
		WeaponMagazineBehavior* pScript = pObj->GetScript<WeaponMagazineBehavior>();

		if (pScript)
		{
			Ammos* pAmmoToDisplay = pInventory->GetAmmos(pScript->GetAmmoTypeFromWeapon());

			if (pAmmoToDisplay)
			{
				totalAmmoTxt = L"Stock : " + std::to_wstring(pAmmoToDisplay->GetAmount());
			}
			else
			{
				totalAmmoTxt = NOTHING;
			}
		}
		else
		{
			totalAmmoTxt = NOTHING;
		}
	}
	else
	{
		totalAmmoTxt = L"To Do";
	}

	pTotalAmmoUI->UpdateDynamicText(totalAmmoTxt);
}

void UpdateHpUI()
{
	gce::GameObject* pPlayer = GameManager::GetSceneManager().GetFirstGameObject({ Tag::TPlayer });

	if (pPlayer == nullptr)
		return;

	HealthBehavior* health = pPlayer->GetScript<HealthBehavior>();

	if (health)
	{
		hpTxt = L"HP : " + std::to_wstring(health->health) + L"/" + std::to_wstring(health->maxHealth);
		hpBar.SetFilledBarByRatio(health->health, health->maxHealth);
	}
	else
	{
		hpTxt = NOTHING;
	}

	pHpUI->UpdateDynamicText(hpTxt);
}

void Start()
{
	pUIManager = GameManager::GetSceneManager().GetUIManager();

	pFpsUI = &EntityWrapper::Create();
	pFpsUI->AddDynamicTextRenderer(fpsTxt, { 1350, 10, 300, 300 }, gce::Color::Black);

	pAmmoUI = &EntityWrapper::Create();
	gce::Vector3f32 ammoPos = { 1480, 900, 0.f };
	pAmmoUI->AddDynamicTextRenderer(ammoTxt, { ammoPos.x, ammoPos.y, 0, 0 }, gce::Color::Red);

	pTotalAmmoUI = &EntityWrapper::Create();
	gce::Vector3f32 totalAmmmoPos = { 1480, 850, 0.f };
	pTotalAmmoUI->AddDynamicTextRenderer(totalAmmoTxt, { totalAmmmoPos.x, totalAmmmoPos.y, 0, 0 }, gce::Color::Red);

	pHpUI = &EntityWrapper::Create();
	pHpUI->AddDynamicTextRenderer(hpTxt, { 0, 900, 400, 0 }, gce::Color::Green);


	pCrosshair = &EntityWrapper::Create(); // TEST
	Vector2f32 center = { (float)WINDOW_WIDTH / 2.f, (float)WINDOW_HEIGHT / 2.f - 20 };
	pCrosshair->AddUIButton(center, { 0, 0 }, {150, 150}, "res/2D_Assets/crosshair.png");

	hpBar.InitFilledBar("res/2D_Assets/hpBar.png", {405, 53}, { 190, 69}, { 0.5, 0.5 });
	hpBar.InitFrame("res/2D_Assets/hpBar_frame.png", { 569, 204 }, { 160, 60 }, { 0.5, 0.5 });
}

void Update()
{
	bool display = pUIManager->IsSceneType(SceneType::GamePlayScene);

	pAmmoUI->SetActive(display);
	pTotalAmmoUI->SetActive(display);
	pHpUI->SetActive(display);
	pFpsUI->SetActive(display);
	pCrosshair->SetActive(display);
	hpBar.SetActive(display);

	if (display == true)
	{
		UpdateFpsUI();
		UpdateAmmosUI();
		UpdateTotalAmmoUI();
		UpdateHpUI();
	}

}

END_SCRIPT
