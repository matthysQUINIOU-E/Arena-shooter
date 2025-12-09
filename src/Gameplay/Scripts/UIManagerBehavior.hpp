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

using namespace gce;

DECLARE_SCRIPT(UIManagerBehavior, ScriptFlag::Start | ScriptFlag::Update)

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

std::wstring NOTHING = L"";


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

	if (pObj->IsTag1(PrimaryTag::TWeapon))
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

	pAmmoUI->GetComponent<TextRenderer>()->text = ammoTxt;
}

void UpdateTotalAmmoTxt()
{
	InventoryManager* pInventory = GameManager::GetSceneManager().GetInventoryManager();

	gce::GameObject* pObj = pInventory->GetCurrentEquipedObject();

	if (pObj == nullptr)
	{
		return;
	}

	if (pObj->IsTag1(PrimaryTag::TWeapon))
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

	pTotalAmmoUI->GetComponent<TextRenderer>()->text = totalAmmoTxt;
}

void UpdateHpTxt()
{
	gce::GameObject* pPlayer = GameManager::GetSceneManager().GetFirstGameObject(PrimaryTag::TPlayer, SecondaryTag::None);

	if (pPlayer == nullptr)
		return;

	HealthBehavior* health = pPlayer->GetScript<HealthBehavior>();

	if (health)
	{
		hpTxt = L"HP : " + std::to_wstring(health->health) + L"/" + std::to_wstring(health->maxHealth);
	}
	else
	{
		hpTxt = NOTHING;
	}

	pHpUI->GetComponent<TextRenderer>()->text = hpTxt;
}

void Start()
{
	pFpsUI = &EntityWrapper::Create();
	pFpsUI->AddTextRenderer(L"Fps", { 0, 0, 300, 300 }, gce::Color::Black);

	pAmmoUI = &EntityWrapper::Create();
	gce::Vector3f32 ammoPos = { 1480, 900, 0.f };
	pAmmoUI->AddTextRenderer(L"Ammos", { ammoPos.x, ammoPos.y, 0, 0 }, gce::Color::Red);

	pTotalAmmoUI = &EntityWrapper::Create();
	gce::Vector3f32 totalAmmmoPos = { 1480, 850, 0.f };
	pTotalAmmoUI->AddTextRenderer(L"Total Ammos", { totalAmmmoPos.x, totalAmmmoPos.y, 0, 0 }, gce::Color::Red);

	pHpUI = &EntityWrapper::Create();
	pHpUI->AddTextRenderer(L"HP", { 0, 850, 400, 0 }, gce::Color::Red);
}

void Update()
{
	UpdateFPSTxt();
	UpdateAmmoTxt();
	UpdateTotalAmmoTxt();
	UpdateHpTxt();
}

END_SCRIPT
