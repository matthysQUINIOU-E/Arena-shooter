#pragma once

#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "BulletBehavior.hpp"
#include "../Prefabs/EntityWrapper.h"

using namespace gce;

DECLARE_SCRIPT(GunBehavior, ScriptFlag::Start | ScriptFlag::Update)

void Shoot()
{
	gce::Scene& scene = GameManager::GetScene();

	EntityWrapper& bullet = EntityWrapper::Create();

	if (m_pOwner->GetChildren().Empty())
	{
		return;
	}

	gce::Vector3f32 spawnPoint = m_pOwner->GetChildren()[0]->transform.GetWorldPosition();

	bullet.SetProperties("Bullet", Tag::TProjectile, spawnPoint, { 0, 0, 0 }, { 0.15, 0.15, 0.15 });
	bullet.AddMeshRenderer(gce::SHAPES.SPHERE, "");

	bullet.AddComponent<SphereCollider>();
	bullet.AddScript<BulletBehavior>()->SetGun(m_pOwner);
}

void Start()
{
}

void Update()
{
	
}

END_SCRIPT