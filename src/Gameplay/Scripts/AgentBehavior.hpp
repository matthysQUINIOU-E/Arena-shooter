#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "Components.h"
#include "Agent.h"

using namespace gce;

DECLARE_SCRIPT(AgentBehavior, ScriptFlag::Start | ScriptFlag::Update)

//Members
Agent* pAgent = nullptr;

void Start()
{
	pAgent = dynamic_cast<Agent*>(m_pOwner);
	if (pAgent == nullptr)
		return;
}

void Update()
{
	if (pAgent == nullptr)
		return;

	pAgent->FollowPathToTarget();
}

END_SCRIPT
