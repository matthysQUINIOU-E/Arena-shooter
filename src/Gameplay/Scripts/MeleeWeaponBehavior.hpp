#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "../Prefabs/EntityWrapper.h"
#include "../KeyBinds.h"

using namespace gce;

DECLARE_SCRIPT(MeleeWeaponBehavior, ScriptFlag::Start | ScriptFlag::Update)

bool isHitting = false;
float unloadSpeed = 0.5f;
float unloadProgress = unloadSpeed;

float gettingWeaponTime = 0.5f; // To prevent switching very fast weapon to shoot instantly
float gettingWeaponProgressTime = 0.f;

Quaternion defaultRotation;
int damage = 0;

void SetDefaultRotation(Quaternion rot) { defaultRotation = rot; }

const bool& IsReadyToUse() const
{
	return gettingWeaponProgressTime >= gettingWeaponTime;
}
void OnLeaveWeapon() // When this weapon will be changed
{
	m_pOwner->transform.SetLocalRotation(defaultRotation);
	unloadProgress = unloadSpeed;
}
void OnReceiveWeapon() // When this weapon will be the new current one
{
	gettingWeaponProgressTime = 0.f;
}

// IN CLASS METHODS, DO NOT USE SOMEWHERE ELSE
void HandleGettingWeaponAnimation(float dt, MeshRenderer* pMesh)
{
	gce::Vector3f32 dir = m_pOwner->transform.GetLocalForward();

	float ratio = gettingWeaponProgressTime / gettingWeaponTime;
	float valueRatio = (gce::PI / 4) * (1 - ratio);

	Quaternion rotation = {};
	rotation.SetRotationAxis(dir * GameManager::GetSceneManager().GetCameraObject()->transform.GetWorldForward(), valueRatio);

	m_pOwner->transform.SetLocalRotation(rotation * defaultRotation);

	gettingWeaponProgressTime += dt;
}

void HandleHitAnimation(float dt)
{
	gce::Vector3f32 dir = m_pOwner->transform.GetLocalForward();

	Quaternion rotation = {};

	float ratio = unloadProgress / unloadSpeed;

	ratio = cos(ratio);

	rotation.SetRotationAxis(dir, -gce::PI / 3 * ratio);

	m_pOwner->transform.SetLocalRotation(rotation * defaultRotation);
}
///////////////////////////////////////////////

void SetMeleeWeaponProperties(int _damage, float hittingDuration)
{
	damage = _damage;
	unloadSpeed = hittingDuration;
	unloadProgress = unloadSpeed;
}

void SetUnloadSpeed(float speed)
{
	unloadSpeed = std::abs(speed);
	unloadProgress = unloadSpeed;
}

void Hit()
{
	if (isHitting || unloadProgress < unloadSpeed)
		return;

	isHitting = true;
	unloadProgress = 0.f;
}

void Start()
{
}

void Update()
{
	float dt = GameManager::DeltaTime();
	MeshRenderer* pMesh = m_pOwner->GetComponent<MeshRenderer>();

	if (pMesh == nullptr)
		return;

	if (IsReadyToUse() == false)
	{
		HandleGettingWeaponAnimation(dt, pMesh);
		return;
	}

	pMesh->pMaterial->useTextureAlbedo = 1;

	if (isHitting)
	{
		if (unloadProgress < unloadSpeed)
		{
			unloadProgress += dt;
			HandleHitAnimation(dt);
			return;
		}
		else
		{
			isHitting = false;
		}
	}
	else
	{
		m_pOwner->transform.SetLocalRotation(defaultRotation);
	}
}

END_SCRIPT