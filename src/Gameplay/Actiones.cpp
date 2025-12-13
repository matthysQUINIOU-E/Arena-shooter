#include "Actiones.h"

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

	if (m_reloadAmmo < m_baseReloadAmmo)
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
