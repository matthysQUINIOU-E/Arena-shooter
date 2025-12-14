#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "../Prefabs/InventoryManager.h"
#include "../Prefabs/BulletPool.h"
#include <limits>

using namespace gce;

DECLARE_SCRIPT(BulletBehavior, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::Destroy)

//Members
float maxLifeTime = 3.f;
float lifeTime = maxLifeTime;
float speed = 0.f;
int damage = 1;
bool headseeker = false;

gce::Vector3f32 defaultDir = {};
gce::Vector3f32 dir = {};

void ActiveHeadSeeker()
{
	headseeker = true;
}

void Start()
{
	speed = 10.f;
}

void Reset()
{
	speed = 0.f;
	damage = 1;
	lifeTime = maxLifeTime;
	defaultDir = {};
	dir = {};
	headseeker = false;

	BulletPool::Desactive(m_pOwner);
}

gce::GameObject* GetNearestEnemy()
{
	auto enemies = GameManager::GetSceneManager().GetAllGameObjects({ Tag::TEnemy });
	float minSqrDistance = std::numeric_limits<float>::infinity();

	gce::GameObject* pNearestEnemy = nullptr;

	float maxDistanceDetection = 2.f;

	for (gce::GameObject* pCurrent : enemies)
	{
		gce::Vector3f32 bulletPos = m_pOwner->transform.GetWorldPosition();
		gce::Vector3f32 enemyPos = pCurrent->transform.GetWorldPosition();

		float sqrDist = (enemyPos.x - bulletPos.x) * (enemyPos.x - bulletPos.x) + (enemyPos.y - bulletPos.y) * (enemyPos.y - bulletPos.y) + (enemyPos.z - bulletPos.z) * (enemyPos.z - bulletPos.z);

		if (sqrDist < minSqrDistance && sqrDist < maxDistanceDetection * maxDistanceDetection)
		{
			minSqrDistance = sqrDist;
			pNearestEnemy = pCurrent;
		}
	}

	return pNearestEnemy;
}

void SeekEnemy(gce::GameObject* pEnemy) // Change the direction
{
	if (pEnemy == nullptr)
	{
		dir = defaultDir;
		return;
	}

	gce::Vector3f32 bulletPos = m_pOwner->transform.GetWorldPosition();
	gce::Vector3f32 enemyPos = pEnemy->transform.GetWorldPosition();

	dir = enemyPos - bulletPos;
	dir.SelfNormalize();
}

void Update()
{
	float dt = GameManager::DeltaTime();

	if (lifeTime < 0)
	{
		Reset();
	}
	else
	{
		if (headseeker == true)
		{
			SeekEnemy(GetNearestEnemy());
		}

		lifeTime -= dt;

		m_pOwner->transform.WorldTranslate(dir * speed * dt);
	}
}

void Destroy()
{
}

void CollisionEnter(GameObject* other)
{
	if (other->HasTags({ Tag::TPlayer }))
		return;

	Reset();
}

void CollisionExit(GameObject* other)
{
}
END_SCRIPT
