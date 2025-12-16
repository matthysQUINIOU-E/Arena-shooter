#include "Actiones.h"
#include "Scripts/EnemyProjectileBehavior.hpp"
#include "Scripts/HealthBehavior.hpp"

ReloadAmmo::ReloadAmmo(int* ammo, int* maxAmmo, float baseReloadAmmo)
{
	m_maxAmmo = maxAmmo;
	m_ammo = ammo;
	m_baseReloadAmmo = baseReloadAmmo;
	m_reloadAmmo = 0.f;
}

void ReloadAmmo::Update(float deltaTime)
{
	if (*m_ammo == *m_maxAmmo)
		return;

	if (m_reloadAmmo >= m_baseReloadAmmo)
	{
		m_reloadAmmo = 0.f;
		(*m_ammo)++;
	}
	else
		m_reloadAmmo += deltaTime;
}

ChangeAgentRange::ChangeAgentRange(Agent* agent, int* ammo, float meleeRange, float distanceRange)
{
	m_ammo = ammo;
	m_agent = agent;
	m_meleeRange = meleeRange;
	m_distanceRange = distanceRange;
}

void ChangeAgentRange::Update(float deltaTime)
{
	if (*m_ammo == 0)
		m_agent->SetStopRange(m_meleeRange);
	else
		m_agent->SetStopRange(m_distanceRange);
}

MeleeAttack::MeleeAttack(Agent* agent, int damage, int hitRange, float attackHitFrame, float baseAttackCooldown, bool* attackLaunched)
{
	m_agent = agent;
	m_hitRange = hitRange;
	m_attackHitFrame = attackHitFrame;
	m_baseAttackCooldown = baseAttackCooldown;
	m_attackCooldown = 0.f;
	m_attackLaunched = attackLaunched;
	m_attackReady = true;
	m_currentAttackFrame = 0.f;
	m_damage = damage;
}


void MeleeAttack::Update(float deltaTime)
{
	if (m_attackReady)
	{
		(*m_attackLaunched) = true;
		m_attackReady = false;
	}

	if ((*m_attackLaunched))
	{
		m_currentAttackFrame += deltaTime;
	}

	if (m_currentAttackFrame >= m_attackHitFrame)
	{
		(*m_attackLaunched) = false;
		m_currentAttackFrame = 0.f;
		if (m_agent->GetDistanceFromTarget() <= m_hitRange)
			GameManager::GetSceneManager().GetFirstGameObject({ Tag::TPlayer })->GetScript<HealthBehavior>()->TakeDamage(m_damage);
	}

	if (!m_attackReady && !(*m_attackLaunched))
	{
		m_attackCooldown += deltaTime;
		if (m_attackCooldown >= m_baseAttackCooldown)
		{
			m_attackReady = true;
			m_attackCooldown = 0.f;
		}
	}
}

void MeleeAttack::Reset()
{
	m_attackReady = true;
	(*m_attackLaunched) = false;
	m_currentAttackFrame = 0.f;
	m_attackCooldown = 0.f;
}

DistanceAttack::DistanceAttack(Agent* agent, float baseAttackCooldown, int* ammo, std::vector<gce::GameObject*>* projectiles)
{
	m_agent = agent;
	m_baseAttackCooldown = baseAttackCooldown;
	m_attackCooldown = 0.f;
	m_ammo = ammo;
	m_attackReady = true;
	m_projectiles = projectiles;
}

void DistanceAttack::Update(float deltaTime)
{
	if (m_attackReady && (*m_ammo) > 0)
	{
		m_attackReady = false;
		if (m_projectiles->empty())
		{
			std::vector<gce::GameObject*> getLauched;
			for (gce::GameObject* proj : m_lauchedProjectiles)
			{
				if (!proj->IsActive())
					getLauched.push_back(proj);
			}

			for (size_t i = 0; i < getLauched.size(); i++)
			{
				gce::GameObject* proj = getLauched[i];
				m_lauchedProjectiles.erase(proj);
				m_projectiles->push_back(proj);
			}
		}
	
		gce::GameObject* proj = m_projectiles->back();
		EnemyProjectileBehavior* epb = proj->GetScript<EnemyProjectileBehavior>();
		epb->Shoot(m_agent->transform.GetWorldPosition(), m_agent->GetTarget()->transform.GetWorldPosition());
		m_projectiles->pop_back();
		m_lauchedProjectiles.insert(proj);

		(*m_ammo)--;
	}

	if (!m_attackReady)
	{
		m_attackCooldown += deltaTime;
		if (m_attackCooldown >= m_baseAttackCooldown)
			Reset();
	}
}

void DistanceAttack::Reset()
{
	m_attackCooldown = 0.f;
	m_attackReady = true;
}
