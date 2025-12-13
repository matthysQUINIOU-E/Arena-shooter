#pragma once

#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "HealthBehavior.hpp"
#include "../SceneManager.h"
#include "Prefabs/Player.h"

using namespace gce;

DECLARE_SCRIPT(BonusManagerBehavior, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::Destroy)

//Members

float rotationSpeed = 2.f;
int healValue = 0;

void SetProperties(int _healValue)
{
	healValue = _healValue;
}

void Start()
{
	m_pOwner->transform.SetWorldRotation({ gce::PI / 16, 0, 0 });
}

void Update()
{
	float dt = GameManager::DeltaTime();
	float val = rotationSpeed * dt;

	m_pOwner->transform.WorldRotate({0, val, 0});
}

void Destroy()
{
}

void CollisionEnter(GameObject* other)
{
	if (other->HasTags({ Tag::TPlayer }))
	{
		other->GetScript<HealthBehavior>()->Heal(healValue);
		m_pOwner->Destroy();
	}
}

END_SCRIPT
