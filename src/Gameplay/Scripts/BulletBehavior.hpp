#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "../Prefabs/InventoryManager.h"
#include "../Prefabs/BulletPool.h"
#include "../Scripts/HealthBehavior.hpp"

#include <limits>

using namespace gce;

DECLARE_SCRIPT(BulletBehavior, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::Destroy)

//Members
float maxLifeTime = 3.f;
float lifeTime = maxLifeTime;
float speed = 0.f;
int damage = 1;
bool headseeker = false;

float easingTime = 0.1f; // The more the value is, the less the headseeker will be efficient
float easingProgressTime = 0.f;

gce::Vector3f32 defaultDir = {};
gce::Vector3f32 dir = {};

bool triggerAnim = false;
float animDuration = 0.25f;
float animProgressDuration = 0.f;

gce::GameObject* CheckCollision()
{
	for (gce::GameObject* go : GameManager::GetSceneManager().GetAllGameObjects({ Tag::TEnemy }))
	{
		gce::Vector3f32 pos1 = m_pOwner->transform.GetWorldPosition();
		gce::Vector3f32 scale1 = m_pOwner->transform.GetWorldScale();

		gce::Vector3f32 pos2 = go->transform.GetWorldPosition();
		gce::Vector3f32 scale2 = go->transform.GetWorldScale();

		bool collision =
			std::abs(pos1.x - pos2.x) <= (scale1.x * 0.5f + scale2.x * 0.5f) &&
			std::abs(pos1.y - pos2.y) <= (scale1.y * 0.5f + scale2.y * 0.5f) &&
			std::abs(pos1.z - pos2.z) <= (scale1.z * 0.5f + scale2.z * 0.5f);

		if (collision)
			return go;
	}

	return nullptr;
}

void ActiveHeadSeeker()
{
	headseeker = true;
}

void Start()
{
}

void Reset()
{
	speed = 0.f;
	damage = 1;
	lifeTime = maxLifeTime;
	defaultDir = {};
	dir = {};
	headseeker = false;

	triggerAnim = false;
	animProgressDuration = 0.f;

	BulletPool::Desactive(m_pOwner);
}

gce::GameObject* GetNearestEnemy()
{
	auto enemies = GameManager::GetSceneManager().GetAllGameObjects({ Tag::TEnemy });
	float minSqrDistance = std::numeric_limits<float>::infinity();

	gce::GameObject* pNearestEnemy = nullptr;

	float maxDistanceDetection = 5.f;

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

	if (easingProgressTime < easingProgressTime)
	{
		easingProgressTime += GameManager::DeltaTime();
		return;
	}
	else
	{
		easingProgressTime = 0.f;

		gce::Vector3f32 bulletPos = m_pOwner->transform.GetWorldPosition();
		gce::Vector3f32 enemyPos = pEnemy->transform.GetWorldPosition();

		dir = enemyPos - bulletPos;
		dir.SelfNormalize();
	}
}

void Update()
{
	float dt = GameManager::DeltaTime();

	if (triggerAnim)
	{
		if (animProgressDuration < animDuration)
		{
			animProgressDuration += dt;

			float val = 2 * dt;

			m_pOwner->transform.WorldScale({ 1 + val, 1 + val, 1 + val });
		}
		else
		{
			Reset();
		}

		return;
	}

	if (lifeTime < 0)
	{
		triggerAnim = true;
	}
	else
	{
		if (headseeker == true)
		{
			SeekEnemy(GetNearestEnemy());
		}

		lifeTime -= dt;

		float yaw = atan2(dir.x, dir.z);
		float pitch = atan2(
			-dir.y,
			sqrt(dir.x * dir.x + dir.z * dir.z)
		);

		gce::Quaternion rot = {};
		rot.SetRotationEuler(pitch, yaw, 0.0f);

		Quaternion flip = {};
		flip.SetRotationEuler({ 0, gce::PI, 0 });

		m_pOwner->transform.SetWorldRotation(flip * rot);

		m_pOwner->transform.WorldTranslate(dir * speed * dt);
	}

	if (gce::GameObject* pCollided = CheckCollision())
	{
		if (pCollided->HasTags({ Tag::TEnemy }))
		{
			if (auto health = pCollided->GetScript<HealthBehavior>())
			{
				health->TakeDamage(damage);
			}
		}

		triggerAnim = true;
	}
}

void Destroy()
{
}

void CollisionEnter(GameObject* other)
{

}

END_SCRIPT
