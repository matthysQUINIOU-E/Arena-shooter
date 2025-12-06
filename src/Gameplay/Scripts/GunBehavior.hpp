#pragma once

#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "BulletBehavior.hpp"
#include "../Prefabs/EntityWrapper.h"
#include "AmmoManagerBehavior.hpp"

using namespace gce;

DECLARE_SCRIPT(GunBehavior, ScriptFlag::Start | ScriptFlag::Update)

float unloadSpeed = 0.1f;
float unloadProgress = 0.f;

float reloadTime = 1.f;
float reloadProgressTime = 0.f;

bool isReloading = false;

AmmoManagerBehavior* ammoManagerScript = nullptr;

Quaternion defaultRotation;

void SetAmmoManagerScript(AmmoManagerBehavior* script) { ammoManagerScript = script; }

void SetUnloadSpeed(float speed)
{
	unloadSpeed = std::abs(speed);
}

void SetReloadTime(float newTime)
{
	reloadTime = std::abs(newTime);
}

void Reload()
{
	if (ammoManagerScript == nullptr)
		return;

	if (isReloading == false && ammoManagerScript->IsFullAmmos() == false)
	{
		isReloading = true;
		reloadProgressTime = 0.f;
	}
}

void Shoot()
{
	if (ammoManagerScript == nullptr)
		return;

	//Conditions to shoot
	if (isReloading || ammoManagerScript->HaveAmmos() == false)
		return;

	//Cap the shooting speed
	if (unloadProgress < unloadSpeed)
		return;

	gce::Scene& scene = GameManager::GetScene();

	EntityWrapper& bullet = EntityWrapper::Create();

	if (m_pOwner->GetChildren().Empty()) // No Spawn point ?
	{
		return;
	}

	gce::Vector3f32 spawnPoint = m_pOwner->GetChildren()[0]->transform.GetWorldPosition();

	bullet.SetProperties("Bullet", Tag1::TProjectile, Tag2::None, spawnPoint, { 0, 0, 0 }, { 0.15, 0.15, 0.15 });
	bullet.AddMeshRenderer(gce::SHAPES.SPHERE, "");

	bullet.AddComponent<SphereCollider>();
	bullet.AddScript<BulletBehavior>()->SetGun(m_pOwner);

	unloadProgress = 0.f;
	ammoManagerScript->UseAmmos();
}

void Start()
{
	defaultRotation = m_pOwner->transform.GetLocalRotation();
}

void Update()
{
	if (m_pOwner->IsActive() == false)
		return;

	if (GetKeyDown(Keyboard::R)) // TODO encapsulate the keybinds
		Reload();

	float dt = GameManager::DeltaTime();

	if (isReloading == false && ammoManagerScript->HaveAmmos() == false)
	{
		m_pOwner->GetComponent<MeshRenderer>()->pMaterial->useTextureAlbedo = 0;
	}

	if (isReloading)
	{
		m_pOwner->GetComponent<MeshRenderer>()->pMaterial->useTextureAlbedo = 1.f;

		m_pOwner->transform.LocalRotate({ 35 * dt, 0, 0 });
		if (reloadProgressTime < reloadTime)
		{
			reloadProgressTime += dt;
		}
		else
		{
			isReloading = false;
			reloadProgressTime = 0.f;
			ammoManagerScript->FillAmmos();
		}
	}
	else if(isReloading == false && ammoManagerScript->HaveAmmos() == true)
	{
		m_pOwner->transform.SetLocalRotation(defaultRotation);

		if (unloadProgress < unloadSpeed)
		{
			unloadProgress += dt;
		}
	}
}

END_SCRIPT