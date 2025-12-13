#include "Agent.h"

class Conditione
{
public:
	virtual bool Get() { return true; }
};

class NeedToMove : public Conditione
{
public:
	NeedToMove(Agent* agent, float rangeMelee, float rangeDistance);
	bool Get() override;
private:
	float m_rangeMelee;
	float m_rangeDistance;
	Agent* m_agent;
};

class CanMeleeAttack : public Conditione
{
public:
	CanMeleeAttack(Agent* agent, float meleeRange);

	bool Get() override;
private:
	Agent* m_agent;
	float m_meleeRange;
};

class CanDistanceAttack : public Conditione
{
public:
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
	CantAttack(Agent* agent, int* ammo, float distanceRange);

	bool Get() override;
private:
	Agent* m_agent;
	int* m_ammo;
	float m_distanceRange;
};
