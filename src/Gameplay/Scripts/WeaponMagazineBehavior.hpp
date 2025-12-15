#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "GameManager.h"
#include "../SceneManager.h"
#include "../Prefabs/InventoryManager.h"
#include "../Prefabs/Ammos.h"
#include "../Tags.h"

using namespace gce;

DECLARE_SCRIPT(WeaponMagazineBehavior, ScriptFlag::Start | ScriptFlag::Update)

int ammosLeft = 0;
int maxCapacity = 0;

const Tag& GetAmmoTypeFromWeapon()
{
	Tag typeToUse = Tag::None;

	if (m_pOwner)
	{
		switch (m_pOwner->GetUniqueTag({ Tag::TMusket, Tag::TBlunderBuss, Tag::TStarwheel }))
		{
		case Tag::TMusket:
			typeToUse = Tag::TNormalAmmo;
			break;
		case Tag::TBlunderBuss:
			typeToUse = Tag::THeavyAmmo;
			break;
		case Tag::TStarwheel:
			typeToUse = Tag::TLightAmmo;

		default:
			break;
		}
	}

	return typeToUse;
}

const bool& IsWeaponEmpty() const
{
	if (ammosLeft <= 0)
		return false;

	return true;
}

const bool& IsWeaponFull() const
{
	if (ammosLeft >= maxCapacity)
		return true;

	return false;
}

void SetMaxCapacity(int val) { maxCapacity = std::abs(val); ammosLeft = maxCapacity; }

void UseWeaponAmmo(int count = 1)
{
	ammosLeft = std::clamp(ammosLeft - count, 0, maxCapacity);
}

void FillWeaponAmmos()
{
	ammosLeft = maxCapacity;
}

bool CanReload() // Pick ammos from your inventory ammo stock if possible and trigger the gun reload
{
	if (ammosLeft >= maxCapacity)
		return false;

	Ammos* ammoToDecrease = GameManager::GetSceneManager().GetInventoryManager()->GetAmmos(GetAmmoTypeFromWeapon());

	if (ammoToDecrease->IsEmpty())
		return false;

	return true;
}

void Start()
{
}

void Update()
{
}

END_SCRIPT