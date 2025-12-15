#pragma once

#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include <functional>
#include "Components.h"
#include "Scripts/HealthBehavior.hpp"

using namespace gce;

DECLARE_SCRIPT(EnemyProjectileBehavior, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

float m_baseLifeTime = 3.f;
float m_lifeTime = 0.f;
float m_speed = 75.f;
int m_damage = 1;
float m_ySpawnOffset = 0.f;
std::function<void(float, gce::Vector3f32)> m_animation = [](float deltaTime, gce::Vector3f32 dir){};

gce::Vector3f32 m_dir = { 0, 0, 0 };

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
}

void Shoot(gce::Vector3f32 from, gce::Vector3f32 toward)
{
	from.y += m_ySpawnOffset;
	m_pOwner->transform.SetWorldPosition(from);
	m_dir = (toward - from).Normalize();
	m_lifeTime = m_baseLifeTime;
	m_pOwner->SetActive(true);
}

void CollisionEnter(GameObject* other)
{
	if (other->IsTags({ Tag::TPlayer }))
		other->GetScript<HealthBehavior>()->TakeDamage(m_damage);
	m_pOwner->SetActive(false);
}

END_SCRIPT
