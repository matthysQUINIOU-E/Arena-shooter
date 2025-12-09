#pragma once

#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "../Prefabs/InventoryManager.h"

using namespace gce;

DECLARE_SCRIPT(BulletBehavior, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::CollisionStay | ScriptFlag::CollisionExit | ScriptFlag::Destroy)

//Members
GameObject* pWeapon = nullptr;

float lifeTime = 3.f;
float speed = 75.f;

void SetWeapon(GameObject* go) { pWeapon = go; }

gce::Vector3f32 dir = {0, 0, 0};

void Start()
{
	if (pWeapon == nullptr)
		return;

	dir = -pWeapon->transform.GetWorldForward();
}

void Update()
{
	SetWeapon(GameManager::GetSceneManager().GetInventoryManager()->GetCurrentEquipedObject());

	if (pWeapon == nullptr || pWeapon->IsTag1(PrimaryTag::TWeapon) == false)
		return;

	float dt = GameManager::DeltaTime();

	if (lifeTime < 0)
	{
		m_pOwner->Destroy();
	}
	else
	{
		lifeTime -= dt;

		m_pOwner->transform.WorldTranslate(dir * speed * dt);
	}
}

void Destroy()
{
}

void CollisionStay(GameObject* other)
{
	if (other->IsTag1(PrimaryTag::TPlayer))
		return;

	m_pOwner->Destroy();
}

void CollisionEnter(GameObject* other)
{
	if (other->IsTag1(PrimaryTag::TPlayer))
		return;

	m_pOwner->Destroy();
}

void CollisionExit(GameObject* other) override
{
}
END_SCRIPT
