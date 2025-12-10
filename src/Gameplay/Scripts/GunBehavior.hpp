#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"

#include "BulletBehavior.hpp"
#include "../Prefabs/EntityWrapper.h"
#include "WeaponMagazineBehavior.hpp"
#include "../Prefabs/Ammos.h"

using namespace gce;

DECLARE_SCRIPT(GunBehavior, ScriptFlag::Start | ScriptFlag::Update)

float unloadSpeed = 0.1f;
float unloadProgress = 0.f;

float reloadTime = 1.f;
float reloadProgressTime = 0.f;

bool isReloading = false;

WeaponMagazineBehavior* pMagazineBehavior = nullptr;
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

void SetAmmoManagerScript(WeaponMagazineBehavior* script) { pMagazineBehavior = script; }

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
	if (pMagazineBehavior == nullptr)
		return;

	if (isReloading == false && pMagazineBehavior->CanReload())
	{
		isReloading = true;
		reloadProgressTime = 0.f;
	}
}

void Shoot()
{
	if (pMagazineBehavior == nullptr)
		return;

	//Conditions to shoot
	if (isReloading || pMagazineBehavior->IsWeaponEmpty() == false)
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

	bullet.SetProperties("Bullet", { Tag::TProjectile }, spawnPoint, { 0, 0, 0 }, { 0.15, 0.15, 0.15 });

	if (m_pOwner->HasTags({ Tag::TBlunderBuss }))
		bullet.transform.LocalScale({ 3, 3, 3 });

	bullet.AddMeshRenderer(gce::SHAPES.SPHERE, "");

	bullet.AddComponent<SphereCollider>();
	bullet.AddScript<BulletBehavior>()->SetWeapon(m_pOwner);

	GameManager::GetSceneManager().LinkObjectToScene(&bullet, SceneType::GamePlayScene);

	unloadProgress = 0.f;
	pMagazineBehavior->UseWeaponAmmo();
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

	MeshRenderer* pMesh = m_pOwner->GetComponent<MeshRenderer>();

	if (pMesh == nullptr)
		return;

	if (isReloading == false)
	{
		m_pOwner->transform.SetLocalRotation(defaultRotation);

		if (pMagazineBehavior->IsWeaponEmpty() == false)
		{
			pMesh->pMaterial->useTextureAlbedo = 0;
		}
		else
		{	
			if (unloadProgress < unloadSpeed)
			{
				unloadProgress += dt;
			}
		}
	}
	else
	{
		pMesh->pMaterial->useTextureAlbedo = 1.f;
		
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

			Ammos* ammoToDecrease = GameManager::GetSceneManager().GetInventoryManager()->GetAmmos(pMagazineBehavior->GetAmmoTypeFromWeapon());

			int amount = pMagazineBehavior->maxCapacity - pMagazineBehavior->ammosLeft;

			int ammoInStock = ammoToDecrease->GetAmount();

			if (ammoInStock > amount)
			{
				pMagazineBehavior->FillWeaponAmmos();
			}
			else
			{
				pMagazineBehavior->ammosLeft += ammoInStock; // Reload the rest
			}

			ammoToDecrease->UseAmmos(amount);
		}
	}
}

END_SCRIPT