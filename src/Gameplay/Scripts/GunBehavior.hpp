#pragma once

#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "BulletBehavior.hpp"
#include "../Prefabs/EntityWrapper.h"
#include "AmmoManagerBehavior.hpp"
#include "Components.h"

using namespace gce;

DECLARE_SCRIPT(GunBehavior, ScriptFlag::Start | ScriptFlag::Update)

float unloadSpeed = 0.1f;
float unloadProgress = 0.f;

float reloadTime = 1.f;
float reloadProgressTime = 0.f;

bool isReloading = false;

AmmoManagerBehavior* pAmmoBehavior = nullptr;
Quaternion defaultRotation;

void DisplayUI()
{
}

void OnLeaveWeapon() // When this weapon will be changed
{
	isReloading = false;
	reloadProgressTime = 0.f;
	m_pOwner->transform.SetLocalRotation(defaultRotation);
	unloadProgress = 0.f;
}

void OnReceiveWeapon() // When this weapon will be the new current one
{
}

void SetAmmoManagerScript(AmmoManagerBehavior* script) { pAmmoBehavior = script; }

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
	if (pAmmoBehavior == nullptr)
		return;

	if (isReloading == false && pAmmoBehavior->IsFullAmmos() == false)
	{
		isReloading = true;
		reloadProgressTime = 0.f;
	}
}

void Shoot()
{
	if (pAmmoBehavior == nullptr)
		return;

	//Conditions to shoot
	if (isReloading || pAmmoBehavior->HaveAmmos() == false)
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

	if(m_pOwner->IsTag2(Tag2::TBlunderBuss))
		bullet.transform.LocalScale({ 3, 3, 3 });

	bullet.AddMeshRenderer(gce::SHAPES.SPHERE, "");

	bullet.AddComponent<SphereCollider>();
	bullet.AddScript<BulletBehavior>()->SetWeapon(m_pOwner);

	unloadProgress = 0.f;
	pAmmoBehavior->UseAmmos();
}

void Start()
{
}

void Update()
{
	defaultRotation.SetIdentity();
	defaultRotation.SetRotationEuler({ 0, gce::PI, 0 });

	if (GetKeyDown(Keyboard::R)) // TODO encapsulate the keybinds
		Reload();

	float dt = GameManager::DeltaTime();

	if (isReloading == false && pAmmoBehavior->HaveAmmos() == false)
	{
		m_pOwner->GetComponent<MeshRenderer>()->pMaterial->useTextureAlbedo = 0;
	}

	if (isReloading)
	{
		m_pOwner->GetComponent<MeshRenderer>()->pMaterial->useTextureAlbedo = 1.f;
		
		int turns = 2; 

		float value = (2.f * gce::PI / reloadTime) * turns;

		m_pOwner->transform.LocalRotate({ value * dt, 0, 0 });
		if (reloadProgressTime < reloadTime)
		{
			reloadProgressTime += dt;
		}
		else
		{
			isReloading = false;
			reloadProgressTime = 0.f;
			pAmmoBehavior->FillAmmos();
		}
	}
	else if(isReloading == false && pAmmoBehavior->HaveAmmos() == true)
	{
		m_pOwner->transform.SetLocalRotation(defaultRotation);

		if (unloadProgress < unloadSpeed)
		{
			unloadProgress += dt;
		}
	}
}

END_SCRIPT