#pragma once
#include "Agent.h"

class Actione
{
public:
	virtual void Update(float deltaTime) {}
	virtual void Reset() {}
};

class ReloadAmmo : public Actione
{
public:
	ReloadAmmo() {}
	ReloadAmmo(int* ammo, int* maxAmmo, float baseReloadAmmo);
	void Update(float deltaTime) override;
private:
	int* m_ammo;
	int* m_maxAmmo;
	float m_baseReloadAmmo;
	float m_reloadAmmo;
};

class ChangeAgentRange : public Actione
{
public:
	ChangeAgentRange() {}
	ChangeAgentRange(Agent* agent, int* ammo, float meleeRange, float distanceRange);
	void Update(float deltaTime) override;
private:
	Agent* m_agent;
	int* m_ammo;
	float m_meleeRange;
	float m_distanceRange;
};

class MeleeAttack : public Actione
{
public:
	MeleeAttack() {}
	MeleeAttack(Agent* agent, int damage, int hitRange, float attackHitFrame, float baseAttackCooldown, bool* attackLaunched);
	void Update(float deltaTime) override;
	void Reset() override;
private:
	Agent* m_agent;
	int m_hitRange;
	float m_attackHitFrame;
	float m_baseAttackCooldown;
	float m_attackCooldown;
	float m_currentAttackFrame;
	bool* m_attackLaunched;
	bool m_attackReady;
	int m_damage;
};

class DistanceAttack : public Actione
{
public:
	DistanceAttack() {}
	DistanceAttack(Agent* agent, float baseAttackCooldown, int* ammo, std::vector<gce::GameObject*>* projectiles);
	void Update(float deltaTime) override;
	void Reset() override;
private:
	Agent* m_agent;
	int* m_ammo;
	float m_baseAttackCooldown;
	float m_attackCooldown;
	bool m_attackReady;
	std::vector<gce::GameObject*>* m_projectiles;
	std::unordered_set<gce::GameObject*> m_lauchedProjectiles;
};