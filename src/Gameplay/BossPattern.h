#pragma once
#include "GameObject.h"
#include <queue>
#include <random>

enum class BossTypeAttack
{
	FIREBALL,
	LASER
};

class BossPattern
{
public:
	BossPattern(gce::GameObject* boss, gce::GameObject* player);
	void Update(float deltaTime);
	void GeneratePattern();
	bool IsPatternFinished();

private:
	void LaunchFireball();
	void LaunchLaser();
	void CreateFireBall();
	void CreateLaser();
	void CheckAttackEnd();
	void CheckAttackCollision();
	void MoveAttacks(float deltaTime);
private:
	struct AttackTimeComparator
	{
		bool operator()(const std::pair<float, BossTypeAttack>& a, const std::pair<float, BossTypeAttack>& b) const
		{
			return a.first > b.first;
		}
	};

	float m_timeSincePatternStart;
	std::priority_queue<std::pair<float, BossTypeAttack>, std::vector<std::pair<float, BossTypeAttack>>, AttackTimeComparator> m_attackToLaunchQueue;

	gce::GameObject* m_boss;
	gce::GameObject* m_player;

	std::vector<gce::GameObject*> m_fireballPool;
	std::vector<gce::GameObject*> m_laserPool;
	std::unordered_set<gce::GameObject*> m_launchedFireball;
	std::unordered_map<gce::GameObject*, gce::Vector3f32> m_mapFireballDir;
	std::unordered_set<gce::GameObject*> m_launchedLaser;

	std::mt19937 m_rng;
	std::uniform_real_distribution<float> m_offSetPositionDistribution;
	std::uniform_real_distribution<float> m_patternTimeDistribution;
	std::uniform_int_distribution<int> m_numberOfFireballDistribution;
	std::uniform_int_distribution<int> m_numberOfLaserDistribution;

	int m_fireBallDamage;
	float m_fireBallSpeed;

	int m_laserDamage;
	float m_damageTickFrequency;
	float m_laserRotationSpeed;
};