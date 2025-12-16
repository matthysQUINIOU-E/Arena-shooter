#pragma once

#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include <functional>
#include "Components.h"
#include "Scripts/HealthBehavior.hpp"
#include "NavMesh.h"

using namespace gce;

DECLARE_SCRIPT(DestructibleBehavior, ScriptFlag::Start)

void Start()
{
	m_baseHitPoint = 3;
	m_hitPoint = 3;
}

void Reset()
{
	m_hitPoint = m_baseHitPoint;
	m_pOwner->SetActive(true);
}

void DestroyFromMesh()
{
	NavMesh::Instance()->DisableObstacle(m_pOwner);
}

void GetHit()
{
	m_hitPoint--;
	if (m_hitPoint <= 0) {
		DestroyFromMesh();
		m_pOwner->SetActive(false);
	}
}


int m_baseHitPoint;
int m_hitPoint;

END_SCRIPT
