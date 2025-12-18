#pragma once
#include "GameObject.h"
#include <queue>

enum class BossTypeAttack
{
	FIREBALL,
	LASER
};

class BossPattern
{
public:
	BossPattern(gce::GameObject* boss, gce::GameObject* player,std::vector<gce::GameObject*> fireballPool, std::vector<gce::GameObject*> laserPool);
	void Update(float deltaTime);
	float GeneratePattern();

private:
	void LaunchFireball();
	void LaunchLaser();
	void CheckAttackEnd();

private:
	float timeSincePatternStart;
	std::queue<std::pair<float, BossTypeAttack>> m_attackToLaunchQueue;

	gce::GameObject* m_boss;
	gce::GameObject* m_player;

	std::vector<gce::GameObject*> m_fireballPool;
	std::vector<gce::GameObject*> m_laserPool;
	std::unordered_set<gce::GameObject*> m_launchedFireball;
	std::unordered_set<gce::GameObject*> m_launchedLaser;
};