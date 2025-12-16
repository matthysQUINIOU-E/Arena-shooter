#include "DragonAction.h"
#include "Scripts/DragonBehavior.hpp"

DragonShoot::DragonShoot(gce::GameObject* dragon ,int* ammo, float CDtime)
{
	m_Dragon = dragon;
	m_ammo = ammo;
	m_CDtime = CDtime;
}

void DragonShoot::Update(float deltaTime)
{

	if (*m_ammo > 0 && m_AttackReady)
	{
		if (m_AttackReady && (*m_ammo) > 0)
		{
			m_AttackReady = false;
			gce::GameObject* proj = m_Dragon->GetScript<DragonBehavior>()->m_FireBalls.back();
			EnemyProjectileBehavior* epb = proj->GetScript<EnemyProjectileBehavior>();
			GameObject* player =  GameManager::GetSceneManager().GetFirstGameObject({Tag::TPlayer });
			epb->Shoot(m_Dragon->transform.GetWorldPosition(), player->transform.GetWorldPosition());
			m_Dragon->GetScript<DragonBehavior>()->m_FireBalls.pop_back();
			m_Dragon->GetScript<DragonBehavior>()->m_launchedFireBalls.insert(proj);

			(*m_ammo)--;
		}

		if (!m_AttackReady)
		{
			m_CDtime += deltaTime;
			if (m_CDtime >= m_BaseAttackCooldown)
				Reset();
		}

	}
}

DragonLaser::DragonLaser(GameObject* Dragon, float ShootTime, float m_LazerPlayerDistance)
{
}
