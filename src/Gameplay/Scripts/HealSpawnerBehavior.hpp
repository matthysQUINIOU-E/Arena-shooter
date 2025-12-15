
#pragma once
#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "../Prefabs/BonusManager.h"

using namespace gce;

DECLARE_SCRIPT(HealSpawnerBehavior, ScriptFlag::Start | ScriptFlag::Update)

GameObject* Noodle = nullptr;
GameObject* Nem = nullptr;
GameObject* RiceBowl = nullptr;
float CDTime = 0.f;

void GenerateHeal()
{
	Noodle->SetActive(false);
	Nem->SetActive(false);
	RiceBowl->SetActive(false);
	int random = rand() & 11;
	if (random == 10)
	{
		Noodle->SetActive(true);
	}
	else if (random >= 7)
	{
		Nem->SetActive(true);
	}
	else
	{
		RiceBowl->SetActive(true);
	}
	CDTime = 60.f;
}
void Start()
{
	gce::Vector3f32 pos = m_pOwner->transform.GetWorldPosition();
	pos.y += 1;
	Noodle = BonusManager::CreateNoodles(pos);
	Nem = BonusManager::CreateNem(pos);
	RiceBowl = BonusManager::CreateRiceBowl(pos);
	GenerateHeal();
}
void Update()
{
	if (!Noodle->IsActive() && !Nem->IsActive() && !RiceBowl->IsActive())
		CDTime -= GameManager::DeltaTime();

	if (CDTime <= 0)
		GenerateHeal();
	
}
END_SCRIPT