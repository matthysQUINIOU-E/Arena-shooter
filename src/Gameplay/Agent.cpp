#include "Agent.h"
#include "GameManager.h"

Agent::Agent() : EntityWrapper()
{
}

Agent& Agent::Create()
{
	Agent* const pNew = new Agent();
	pNew->m_pScene = &gce::GameManager::GetScene();
	pNew->transform.m_pOwner = pNew;
	gce::GameManager::GetLifespanSystem().m_toCreate.gameObjects.Push(pNew);
	return *pNew;
}

void Agent::FollowPathToTarget()
{
	if (IsTargetInRange() || m_pTarget == nullptr )
		return;

	if (NeedCalculatePath())
		FindPath();

	FollowCurrentPath();
}

void Agent::SetTarget(GameObject* target)
{
	m_pTarget = target;
}

void Agent::SetSpeed(float speed)
{
	m_speed = speed;
}

void Agent::SetStopRange(float stopRange)
{
	m_stopRange = stopRange;
}

void Agent::SetCurrentNode(Node<NavTile, Agent>* node)
{
	m_pCurrentNode = node;
}

void Agent::ResetBlockedTime()
{
	m_blockedTime = 0.f;
}

void Agent::FollowCurrentPath() 
{
	if (m_isMoving)
		return;

	if (m_path.size() < 2)
		return;

	if (m_currentPathIndex >= m_path.size() - 1) // should not happen
		return;

	Node<NavTile, Agent>* pToNode = m_path[m_currentPathIndex+1];
	ReleaseTraveledNodes();
	if (!AquireTravelingToNodes(pToNode))
	{
		m_blockedTime += gce::GameManager::DeltaTime();
		return;
	}

	m_blockedTime = 0.f;
	m_pCurrentNode = pToNode;
	GoToPosition(pToNode->data->GetPosition());
}

void Agent::GoToPosition(gce::Vector3f32 position) //TODO
{
}

void Agent::ReleaseTraveledNodes() //TODO
{
}

bool Agent::AquireTravelingToNodes(Node<NavTile, Agent>* goToNode) //TODO
{
	return false;
}

void Agent::FindPath()
{
	m_lastTargetCalculatedPathCoordinates = m_pTarget->transform.GetWorldPosition();
	Node<NavTile, Agent>* pTargetNode = NavMesh::Instance()->GetNearestNodeFromPosition(m_lastTargetCalculatedPathCoordinates);

	if (m_blocked)
		m_path = PathFinder::GetPath({ m_pCurrentNode,pTargetNode }, this);
	else
		m_path = PathFinder::GetPath({ m_pCurrentNode,pTargetNode });

	m_blocked = false;
	m_currentPathIndex = 0;
	m_noPathTime = 0.f;
	m_blocked = 0.f;
}

bool Agent::IsTargetInRange()
{
	gce::Vector3f32 selfPos = transform.GetWorldPosition();
	gce::Vector3f32 targetPos = m_pTarget->transform.GetWorldPosition();
	return (selfPos - targetPos).Norm() < m_stopRange;
}

bool Agent::HasTargetMovedTooMuch()
{
	gce::Vector3f32 targetPos = m_pTarget->transform.GetWorldPosition();
	return (m_lastTargetCalculatedPathCoordinates - targetPos).Norm() > m_stopRange;
}

bool Agent::NeedCalculatePath()
{
	if (m_blockedTime > m_baseBlockedTime)
	{
		m_blocked = true;
		m_blockedTime = 0.f;
		return true;
	}
	
	if (m_path.size() < 2)
		m_noPathTime += gce::GameManager::DeltaTime();

	if (m_noPathTime > m_baseNoPathTime)
	{
		m_noPathTime = 0.f;
		return true;
	}

	return HasTargetMovedTooMuch();
}
