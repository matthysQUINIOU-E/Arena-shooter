#pragma once

#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include <functional>
#include "Components.h"
#include "Scripts/HealthBehavior.hpp"

using namespace gce;

DECLARE_SCRIPT(EnemyProjectileBehavior, ScriptFlag::Start | ScriptFlag::Update)

float m_baseLifeTime = 3.f;
float m_lifeTime = 0.f;
float m_speed = 75.f;
int m_damage = 1;
float m_ySpawnOffset = 2.f;
std::function<void(float, gce::Vector3f32)> m_animation = [](float deltaTime, gce::Vector3f32 dir){};

gce::Vector3f32 m_dir = { 0, 0, 0 };

gce::GameObject* CheckCollision()
{
	for (gce::GameObject* go : GameManager::GetSceneManager().GetAllGameObjects({ Tag::TPlayer }))
	{
		if (go->IsActive() == false)
			continue;

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

void Update()
{
	float dt = GameManager::DeltaTime();

	if (m_lifeTime < 0.f)
	{
		m_pOwner->SetActive(false);
	}
	else
	{
		m_lifeTime -= dt;
		m_pOwner->transform.WorldTranslate(m_dir * m_speed * dt);
		m_animation(dt, m_dir);
	}

	if (gce::GameObject* pCollided = CheckCollision())
	{
		if (pCollided->HasTags({ Tag::TPlayer }))
			pCollided->GetScript<HealthBehavior>()->TakeDamage(m_damage);

		m_pOwner->SetActive(false);
	}
}

void Shoot(gce::Vector3f32 from, gce::Vector3f32 toward)
{
	from.y += m_ySpawnOffset;
	m_pOwner->transform.SetWorldPosition(from);
	m_dir = (toward - from).Normalize();
	m_lifeTime = m_baseLifeTime;
	m_pOwner->SetActive(true);
}

END_SCRIPT
