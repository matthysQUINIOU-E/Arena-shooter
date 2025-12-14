#pragma once
#include "EntityWrapper.h"
#include <queue>

class BulletPool // TODO VERIFY IF IMPOSSIBLE TO OPTIMIZE
{
	static int m_maxAmount;
	static std::queue<EntityWrapper*> m_bulletsFreePool;
public:
	static void SetActive(gce::GameObject* e, bool state);
	static void Init();
	static EntityWrapper* Generate();
	static void Push(EntityWrapper* e);
	static void DesactivateAllBullets();
};

