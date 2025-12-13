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
int m_maxKnives = 3;
int m_currentKnives = 3;
float m_rangeKnives = 7.f;
float m_rangeMelee = 2.f;

StateMachinee m_stateMachine;
ReloadAmmo m_reloadAmmo = ReloadAmmo(&m_currentKnives, &m_maxKnives, m_baseReloadKnives);
ChangeAgentRange m_changeRange = ChangeAgentRange(m_pAgent, &m_currentKnives, m_rangeMelee, m_rangeKnives);

NeedToMove m_needToMove = NeedToMove(m_pAgent, m_rangeMelee, m_rangeKnives);
CanMeleeAttack m_canMeleeAttack = CanMeleeAttack(m_pAgent, m_rangeMelee);
CanDistanceAttack m_canDistanceAttack = CanDistanceAttack(m_pAgent, &m_currentKnives, m_rangeKnives);
CantAttack m_cantAttack = CantAttack(m_pAgent, &m_currentKnives, m_rangeKnives);

void Init() // Start is not working idk why (even with the script flag)
{
	m_initTried = true;
	m_pAgent = dynamic_cast<Agent*>(m_pOwner);
	if (m_pAgent == nullptr)
		return;
	// create Knives


	m_stateMachine.AddStateAction(State::MOVE, m_reloadAmmo);
	m_stateMachine.AddStateAction(State::MOVE, m_changeRange);
	m_stateMachine.AddStateCondition(State::MOVE, State::MELEE_ATTACK, m_canMeleeAttack);
	m_stateMachine.AddStateCondition(State::MOVE, State::DISTANCE_ATTACK, m_canDistanceAttack);

	m_stateMachine.AddStateAction(State::MELEE_ATTACK, m_changeRange);
	m_stateMachine.AddStateCondition(State::MELEE_ATTACK, State::MOVE, m_cantAttack);

	m_stateMachine.AddStateAction(State::DISTANCE_ATTACK, m_changeRange);
	m_stateMachine.AddStateCondition(State::DISTANCE_ATTACK, State::MOVE, m_cantAttack);

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
