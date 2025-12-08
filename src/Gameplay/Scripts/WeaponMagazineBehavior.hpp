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

const SecondaryTag& GetAmmoTypeFromWeapon()
{
	SecondaryTag typeToUse = SecondaryTag::None;

	switch (m_pOwner->GetSecondaryTag())
	{
	case SecondaryTag::TMusket:
		typeToUse = SecondaryTag::TNormalAmmo;
		break;
	case SecondaryTag::TBlunderBuss:
		typeToUse = SecondaryTag::THeavyAmmo;
		break;
	default:
		break;
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