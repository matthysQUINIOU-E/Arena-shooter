#pragma once

#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"

using namespace gce;

DECLARE_SCRIPT(BulletBehavior, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::CollisionStay | ScriptFlag::CollisionExit | ScriptFlag::Destroy)

//Members
GameObject* pGun = nullptr;

float lifeTime = 3.f;
float speed = 50.f;

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
	if (pGun == nullptr)
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
	if (other->GetName() == "Player")
		return;

	m_pOwner->Destroy();
}

void CollisionEnter(GameObject* other)
{
}

void CollisionExit(GameObject* other) override
{
}
END_SCRIPT
