#pragma once
#include "Agent.h"

class Conditione
{
public:
	virtual bool Get() { return true; }
};

class CanMeleeAttack : public Conditione
{
public:
	CanMeleeAttack() {}
	CanMeleeAttack(Agent* agent, float meleeRange);

	bool Get() override;
private:
	Agent* m_agent;
	float m_meleeRange;
};

class CanDistanceAttack : public Conditione
{
public:
	CanDistanceAttack() {}
	CanDistanceAttack(Agent* agent, int* ammo, float distanceRange);

	bool Get() override;
private:
	Agent* m_agent;
	int* m_ammo;
	float m_distanceRange;
};

class CantAttack : public Conditione
{
public:
	CantAttack() {}
	CantAttack(Agent* agent, int* ammo, bool* attacking, float distanceRange);

	bool Get() override;
private:
	Agent* m_agent;
	int* m_ammo;
	bool m_attacking;
	float m_distanceRange;
};
