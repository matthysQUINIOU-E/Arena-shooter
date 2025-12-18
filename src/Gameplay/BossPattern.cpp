#include "BossPattern.h"
#include "Scripts/HealthBehavior.hpp"
BossPattern::BossPattern(gce::GameObject* boss, gce::GameObject* player)
{
	m_boss = boss;
	m_player = player;
	std::random_device rand = std::random_device();
	m_rng = std::mt19937(rand());
	m_offSetDistribution = std::uniform_real_distribution<float>(-5.f, 5.f);
	m_patternTimeDistribution = std::uniform_real_distribution<float>(5.f, 12.f);
	m_numberOfFireballDistribution = std::uniform_int_distribution<int>(1,5);
	m_numberOfLaserDistribution = std::uniform_int_distribution<int>(0, 3);

	m_fireBallDamage = 15;
	m_fireBallSpeed = 15.f;

	m_laserDamage = 5.f;
	m_damageTickFrequency = 0.3f;
	m_laserRotationSpeed = 2.f;
}

void BossPattern::Update(float deltaTime)
{
	m_timeSincePatternStart += deltaTime;

	while (!m_attackToLaunchQueue.empty())
	{
		const auto& [launchTime, attackType] = m_attackToLaunchQueue.top();

		if (launchTime > m_timeSincePatternStart)
			break;

		switch (attackType)
		{
		case BossTypeAttack::FIREBALL:
			LaunchFireball();
			break;

		case BossTypeAttack::LASER:
			LaunchLaser();
			break;
		}

		m_attackToLaunchQueue.pop();
	}
	CheckAttackCollision();
	CheckAttackEnd();
}

float BossPattern::GeneratePattern()
{
	m_attackToLaunchQueue = std::priority_queue<std::pair<float, BossTypeAttack>, std::vector<std::pair<float, BossTypeAttack>>, AttackTimeComparator>();

	m_timeSincePatternStart = 0.0f;
	const float patternDuration = m_patternTimeDistribution(m_rng);

	// HealthBehavior* hb = m_boss->GetScript<HealthBehavior>();
	const float healthValue = 1.0f;// gce::Clamp(hb->health / hb->maxHealth, 0.0f, 1.0f);
	const float aggressivity = 0.7f + (1.0f - healthValue);

	const int fireballCount = static_cast<int>(m_numberOfFireballDistribution(m_rng) * aggressivity);
	const int laserCount = static_cast<int>(m_numberOfLaserDistribution(m_rng) * aggressivity);

	for (int i = 0; i < fireballCount; ++i)
	{
		const float launchTime = std::uniform_real_distribution<float>(0.0f, patternDuration)(m_rng);

		m_attackToLaunchQueue.push({ launchTime, BossTypeAttack::FIREBALL });
	}

	for (int i = 0; i < laserCount; ++i)
	{
		const float launchTime = std::uniform_real_distribution<float>(0.0f, patternDuration)(m_rng);

		m_attackToLaunchQueue.push({ launchTime, BossTypeAttack::LASER });
	}

	return patternDuration;
}

void BossPattern::LaunchFireball()
{
}

void BossPattern::LaunchLaser()
{
}

void BossPattern::CreateFireBall()
{
}

void BossPattern::CreateLaser()
{
}

void BossPattern::CheckAttackEnd() //TODO : regarder si fire ball a passer le sol ou si laser a fini ça rotation (regarde au dessus en gros)
{
}

void BossPattern::CheckAttackCollision()
{
}
