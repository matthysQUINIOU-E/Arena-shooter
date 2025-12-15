
#pragma once
#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "../Prefabs/BonusManager.h"

using namespace gce;

DECLARE_SCRIPT(HealSpawnerBehavior, ScriptFlag::Start | ScriptFlag::Update)

GameObject* HealGenerate;

void Start()
{
	int random = rand() & 11;
	if (random == 10)
	{
		HealGenerate = BonusManager::CreateNoodles(m_pOwner->transform.GetWorldPosition());
	}
	else if (random >= 7)
	{
		HealGenerate = BonusManager::CreateNem(m_pOwner->transform.GetWorldPosition());
	}
	else
	{
		HealGenerate = BonusManager::CreateRiceBowl(m_pOwner->transform.GetWorldPosition());
	}

}
END_SCRIPT