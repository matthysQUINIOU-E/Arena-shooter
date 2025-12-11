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
int damage = 1;

gce::Vector3f32 dir = {};

void Start()
{
	dir = GameManager::GetSceneManager().GetCameraObject()->transform.GetWorldForward();
}

void Update()
{
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
	if (other->HasTags({ Tag::TPlayer }))
		return;

	m_pOwner->Destroy();
}

void CollisionEnter(GameObject* other)
{
	if (other->HasTags({ Tag::TPlayer }))
		return;

	m_pOwner->Destroy();
}

void CollisionExit(GameObject* other) override
{
}
END_SCRIPT
