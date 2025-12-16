#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "Agent.h"
#include "HealthBehavior.hpp"
#include "WaveManager.h"

#include "Scripts/MogwaiBehavior.hpp"
#include "Scripts/GuhuoniaoBehavior.hpp"
#include "Scripts/JiangshiBehavior.hpp"

using namespace gce;

DECLARE_SCRIPT(EnemyDeathBehavior, ScriptFlag::Start | ScriptFlag::Update)

bool animTrigger = false;
float animDuration = 0.5f;
float animProgress = 0.f;

void HandleAnimation()
{
	float dt = GameManager::DeltaTime();

	float value = (gce::PI / 2) * (GameManager::DeltaTime() / animDuration);

	m_pOwner->transform.WorldRotate({ value, 0.f, 0.f });
}

void HandleDeath()
{
	m_pOwner->RemoveComponent<BoxCollider>();

	if (auto mogwaiScript = m_pOwner->GetScript<MogwaiBehavior>())
		mogwaiScript->Reset();

	else if (auto guHuoNiaoScript = m_pOwner->GetScript<GuHuoNiaoBehavior>())
		guHuoNiaoScript->Reset();

	else if (auto jiangshi = m_pOwner->GetScript<JiangshiBehavior>())
		jiangshi->Reset();

	m_pOwner->transform.SetWorldRotation({ 0, 0, 0 });
	m_pOwner->SetActive(false);

	WaveManager::GetInstance()->EnnemyKilled(dynamic_cast<Agent*>(m_pOwner));

	animProgress = 0.f;
	animTrigger = false;
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
