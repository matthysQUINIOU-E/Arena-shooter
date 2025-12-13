#include "Conditiones.h"

NeedToMove::NeedToMove(Agent* agent, float rangeMelee, float rangeDistance)
{
	m_agent = agent;
	m_rangeMelee = rangeMelee;
	m_rangeDistance = rangeDistance;
}

bool NeedToMove::Get()
{
	float distanceFromTarget = m_agent->GetDistanceFromTarget();
	if (distanceFromTarget > m_rangeMelee && distanceFromTarget > m_rangeDistance)
		return true;
	return false;
}

CanMeleeAttack::CanMeleeAttack(Agent* agent, float meleeRange)
{
	m_agent = agent;
	m_meleeRange = meleeRange;
}

bool CanMeleeAttack::Get()
{
	return m_meleeRange == m_agent->GetStopRange() && m_agent->IsTargetInRange();
}

CanDistanceAttack::CanDistanceAttack(Agent* agent, int* ammo, float distanceRange)
{
	m_agent = agent;
	m_ammo = ammo;
	m_distanceRange = distanceRange;
}

bool CanDistanceAttack::Get()
{
	return m_distanceRange == m_agent->GetStopRange() && (*m_ammo) > 0 && m_agent->IsTargetInRange();
}

CantAttack::CantAttack(Agent* agent, int* ammo, float distanceRange)
{
	m_agent = agent;
	m_ammo = ammo;
	m_distanceRange = distanceRange;
}

bool CantAttack::Get()
{
	return !m_agent->IsTargetInRange() || !(m_agent->GetStopRange() == m_distanceRange && (*m_ammo) == 0);
}
