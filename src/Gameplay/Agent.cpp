#include "Agent.h"
#include "GameManager.h"
#include <queue>

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

	if (m_isMoving)
		MoveToTarget();
	else 
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

	if (m_path.size() < 2)
		return;

	if (m_currentPathIndex >= m_path.size() - 1) // should not happen
		return;

	Node<NavTile, Agent>* pToNode = m_path[m_currentPathIndex+1];

	if (m_blockedTime == 0.f) // release only first time
		ReleaseTraveledNodes();

	if (!AcquireTravelingToNodes(pToNode))
	{
		m_blockedTime += gce::GameManager::DeltaTime();
		return;
	}

	m_blockedTime = 0.f;
	m_pCurrentNode = pToNode;
	GoToPosition(pToNode->data->GetPosition());
}

void Agent::MoveToTarget()
{
	gce::Vector3f32 translate = m_direction * m_speed * gce::GameManager::DeltaTime();
	float distanceTraveled = translate.Norm();
	m_distanceToMove -= distanceTraveled;
	if (m_distanceToMove < 0.f)
	{
		transform.SetWorldPosition(m_movingTo);
		m_isMoving = false;
		return;
	}
	transform.WorldTranslate(translate);
}

void Agent::GoToPosition(gce::Vector3f32 position) // TODO :: Rotate toward direction
{
	m_distanceToMove = (transform.GetWorldPosition() - position).Norm();
	m_direction = (transform.GetWorldPosition() - position).Normalize();
	m_movingTo = position;
	m_isMoving = true;
	MoveToTarget();
}

void Agent::ReleaseTraveledNodes()
{
	gce::MeshRenderer* meshRenderer = GetComponent<gce::MeshRenderer>();
	
	if (meshRenderer == nullptr)
		return;

	gce::Vector3f32 pos = transform.GetWorldPosition();
	gce::Vector3f32 min = meshRenderer->pGeometry->min + pos;
	gce::Vector3f32 max = meshRenderer->pGeometry->max + pos;

	for (Node<NavTile, Agent>* node : m_nodesOccupied)
	{
		if (!IsNodeInBounds(node,min,max))
			m_nodesOccupied.erase(node);
	}
}

bool Agent::AcquireTravelingToNodes(Node<NavTile, Agent>* goToNode)
{
	gce::MeshRenderer* meshRenderer = GetComponent<gce::MeshRenderer>();
	if (meshRenderer == nullptr)
		return true;

	if (m_needToAcquire.empty())
	{
		NavMesh::Instance()->ResetVisited();
		std::queue<Node<NavTile, Agent>*> queue;

		gce::Vector3f32 currentPos = transform.GetWorldPosition();
		gce::Vector3f32 ToNodePos = goToNode->data->GetPosition();
		gce::Vector3f32 minPos = { min(currentPos.x,ToNodePos.x), min(currentPos.y,ToNodePos.y), min(currentPos.z,ToNodePos.z) };
		gce::Vector3f32 maxPos = { max(currentPos.x,ToNodePos.x), max(currentPos.y,ToNodePos.y), max(currentPos.z,ToNodePos.z) };
		gce::Vector3f32 min = minPos + meshRenderer->pGeometry->min;
		gce::Vector3f32 max = maxPos + meshRenderer->pGeometry->max;

		queue.push(goToNode);

		while (!queue.empty())
		{
			Node<NavTile, Agent>* testAcquire = queue.front();
			queue.pop();
			if (IsNodeInBounds(testAcquire, min, max))
			{
				m_needToAcquire.push_back(testAcquire);
				for (Node<NavTile, Agent>* neighbor : testAcquire->neighbors)
				{
					if (!neighbor->visited) {
						neighbor->visited = true;
						queue.push(neighbor);
					}
				}
			}
		}
	}

	for (size_t i = 0; i < m_needToAcquire.size(); i++)
	{
		Node<NavTile, Agent>* node = m_needToAcquire[i];
		if (node->occupiedByAgent != nullptr && node->occupiedByAgent != this)
			return false;
	}

	for (size_t i = 0; i < m_needToAcquire.size(); i++)
	{
		Node<NavTile, Agent>* node = m_needToAcquire[i];
		node->occupiedByAgent = this;
		m_nodesOccupied.insert(node);
	}
	m_needToAcquire.clear();
	return true;
}

bool Agent::IsNodeInBounds(Node<NavTile, Agent>* node, gce::Vector3f32 min, gce::Vector3f32 max) // don't check Y axis
{
	return node->data->IsInBounds(min, max);
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
