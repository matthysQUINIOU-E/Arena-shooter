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
GameObject* pGun = nullptr;

float lifeTime = 7.f;
float speed = 100.f;

void SetGun(GameObject* go) { pGun = go; }

gce::Vector3f32 dir = {};

void Start()
{
	if (pGun == nullptr)
		return;

	dir = -pGun->transform.GetWorldForward();
}

void Update()
{
	SetGun(GameManager::GetSceneManager().GetInventoryManager()->GetCurrentEquipedObject());

	if (pGun == nullptr || pGun->IsTag1(Tag1::TWeapon) == false)
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
	if (other->IsTag1(Tag1::TPlayer))
		return;

	m_pOwner->Destroy();
}

void CollisionEnter(GameObject* other)
{
	if (other->IsTag1(Tag1::TPlayer))
		return;

	m_pOwner->Destroy();
}

void CollisionExit(GameObject* other) override
{
}
END_SCRIPT
