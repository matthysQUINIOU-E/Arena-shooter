#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include "Prefabs/ArenaCamera.h"
#include "Components.h"
#include "GunBehavior.hpp"
#include "../SceneManager.h"
#include "../Prefabs/InventoryManager.h"
#include "HealthBehavior.hpp"
#include "KeyBinds.h"
#include <functional>

#include "PlayerBehavior.hpp"
using namespace gce;

DECLARE_SCRIPT(CameraAnimationBehavior, ScriptFlag::Start | ScriptFlag::Update)

gce::Vector3f32 camForward;
Quaternion camRotation;

struct AnimationHandler
{
	bool state;
	float duration;
	float progress;

	AnimationHandler(float _duration)
	{
		duration = _duration;
	}

	void Trigger() { state = true; }
	void Handle()
	{
		if (state == true)
		{
			if (progress < duration)
			{
				progress = std::clamp(progress += GameManager::DeltaTime(), 0.f, duration);
			}
			else
			{
				progress = 0.f;
				state = false;
			}
		}
	}
};

void ShakingAnimation(AnimationHandler& ref)
{
	auto dt = GameManager::DeltaTime();
	float ratio = ref.progress / ref.duration;

	Quaternion rotation = {};
	rotation.SetRotationEuler({ std::cos(ratio) * dt, std::sin(1 - ratio) * dt, 0 });

	m_pOwner->transform.SetWorldRotation(camRotation * rotation);
}

//Members /////////////////////////

std::vector<AnimationHandler*> animations;

AnimationHandler* pShaking;

void TriggerShakingAnimation() { pShaking->state = true; }

//Functions
void Start()
{
	pShaking = new AnimationHandler(1.f);
	animations.push_back(pShaking);
}

void Update()
{
	if (auto pPlayer = m_pOwner->GetParent())
	{
		if (auto script = pPlayer->GetScript<PlayerBehavior>())
		{
			for (AnimationHandler* a : animations)
			{
				a->Handle();
			}

			if (pShaking->state == true)
			{
				ShakingAnimation(*pShaking);
				script->SetLookingAround(false);
			}
			else
			{
				script->SetLookingAround(true);
			}
		}
		else
		{
			camForward = m_pOwner->transform.GetWorldForward();
			camRotation = m_pOwner->transform.GetWorldRotation();
		}
	}
}

END_SCRIPT
