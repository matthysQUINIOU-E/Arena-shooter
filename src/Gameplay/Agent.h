#include "Prefabs/EntityWrapper.h"
#include "PathFinder.h"

class Agent : public EntityWrapper
{

public:
	void FollowPathToTarget();
	void SetTarget(GameObject* target);
	void SetSpeed(float speed);
	void SetStopRange(float stopRange);
	void SetCurrentNode(Node<NavTile, Agent>* node);
	void ResetBlockedTime();
private:
	void FollowCurrentPath();
	void GoToPosition(gce::Vector3f32 position);
	void ReleaseTraveledNodes();
	bool AquireTravelingToNodes(Node<NavTile, Agent>* goToNode);
	void FindPath();
	bool IsTargetInRange();
	bool HasTargetMovedTooMuch();
	bool NeedCalculatePath();
private:
	float m_speed = 5.f;
	float m_stopRange = 20.f;

	float m_baseBlockedTime = 1.f;
	float m_blockedTime = 0.f;
	bool m_blocked = false;

	float m_baseNoPathTime = 3.f;
	float m_noPathTime = 0.f;

	bool m_isMoving = false;

	size_t m_currentPathIndex;
	Node<NavTile, Agent>* m_pCurrentNode;
	std::vector<Node<NavTile, Agent>*> m_path;
	std::vector<Node<NavTile, Agent>*> m_nodesOccupied;

	GameObject* m_pTarget;
	gce::Vector3f32 m_lastTargetCalculatedPathCoordinates;
};