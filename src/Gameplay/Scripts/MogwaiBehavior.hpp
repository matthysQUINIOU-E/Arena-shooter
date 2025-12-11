#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "Agent.h"
#include "StateMachinee.h"

using namespace gce;

DECLARE_SCRIPT(MogwaiBehavior, ScriptFlag::Update)

//Members
bool m_initTried = false;
Agent* m_pAgent = nullptr;
float m_baseReloadKnives = 5.f;
float m_currentReloadKnives = 0.f;
std::vector<gce::GameObject*> m_freeKnives;
int m_maxKnives = 3;
int m_currentKnives = 3;
float m_rangeKnives = 7.f;
float m_rangeMelee = 2.f;
StateMachinee m_stateMachine;

void Init() // Start is not working idk why (even with the script flag)
{
	m_initTried = true;
	m_pAgent = dynamic_cast<Agent*>(m_pOwner);
	if (m_pAgent == nullptr)
		return;
	// create Knives
	

	class CanAttack : public Conditione
	{
	public:
		CanAttack(Agent& agent) : m_agent(agent) {}

		bool Get() override
		{
			return m_agent.IsTargetInRange();
		}
	private:
		Agent& m_agent;
	};

	class NeedToMove : public Conditione
	{
	public:
		NeedToMove(Agent& agent, int& ammo, float rangeMelee, float rangeDistance) : m_agent(agent), m_ammo(ammo)
		{ 
			m_rangeMelee = rangeMelee;
			m_rangeDistance = rangeDistance;
		}

		bool Get() override
		{
			float distanceFromTarget = m_agent.GetDistanceFromTarget();
			if (distanceFromTarget > m_rangeMelee && distanceFromTarget > m_rangeDistance)
				return true;
			return false;
		}
	private:
		float m_rangeMelee;
		float m_rangeDistance;
		int& m_ammo;
		Agent& m_agent;
	};

	class ReloadAmmo : public Actione
	{
	public:
		ReloadAmmo(int& ammo, int& maxAmmo, float baseReloadAmmo) : m_ammo(ammo), m_maxAmmo(maxAmmo)
		{
			m_baseReloadAmmo = baseReloadAmmo;
			m_reloadAmmo = 0.f;
		}
		void Update(float deltaTime) override
		{
			if (m_ammo == m_maxAmmo)
				return;

			if (m_reloadAmmo < m_baseReloadAmmo)
			{
				m_reloadAmmo = 0.f;
				m_ammo++;
			}
			else
				m_reloadAmmo += deltaTime;
		}
	private:
		int& m_ammo;
		int& m_maxAmmo;
		float m_baseReloadAmmo;
		float m_reloadAmmo;
	};

	class ChangeAgentRange : public Actione
	{
	public:
		ChangeAgentRange(int& ammo, float meleeRange, float distanceRange) : m_ammo(ammo)
		{

		}
	private:
		int& m_ammo;
	};



	m_stateMachine.SetState(State::MOVE);

}



void Update()
{
	if (!m_initTried)
		Init();

	if (m_pAgent == nullptr)
		return;


}

END_SCRIPT
