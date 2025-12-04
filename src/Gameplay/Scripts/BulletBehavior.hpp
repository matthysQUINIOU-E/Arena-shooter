#pragma once

#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"

using namespace gce;

DECLARE_SCRIPT(BulletBehavior, ScriptFlag::Start | ScriptFlag::Update)

//Members
float lifeTime = 3.f;
float speed = 50.f;

void Start()
{
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

		m_pOwner->transform.WorldTranslate(-m_pOwner->transform.GetWorldForward() * speed * dt);
	}
}

END_SCRIPT
