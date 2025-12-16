#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "Agent.h"
#include "HealthBehavior.hpp"
#include "WaveManager.h"

using namespace gce;

DECLARE_SCRIPT(EnemyDeathBehavior, ScriptFlag::Start | ScriptFlag::Update)

bool animTrigger = false;
float animDuration = 0.75f;
float animProgress = 0.f;

void HandleAnimation()
{
	float dt = GameManager::DeltaTime();

	Quaternion rot = {};

	float value = (gce::PI / 2) * (GameManager::DeltaTime() / animDuration);

	//rot.SetRotationEuler({value * dt, 0.f, 0.f});

	m_pOwner->transform.WorldRotate({ value, 0.f, 0.f });
}

void HandleDeath()
{
	if (Agent* pAgent = dynamic_cast<Agent*>(m_pOwner))
	{
		pAgent->SetActive(false);
		WaveManager::GetInstance()->EnnemyKilled(pAgent);
	}
}

void Start()
{
}

void Update()
{
	if (animTrigger)
	{
		if (animProgress < animDuration)
		{
			animProgress += GameManager::DeltaTime();
			HandleAnimation();
		}
		else
		{
			animTrigger = false;
			HandleDeath();
		}

		return;
	}

	if (auto health = m_pOwner->GetScript<HealthBehavior>())
	{
		if (health->IsAlive() == false)
		{
			animTrigger = true;
		}
	}
}

END_SCRIPT
