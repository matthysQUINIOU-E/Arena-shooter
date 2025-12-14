#include "BulletPool.h"
#include "../Scripts/BulletBehavior.hpp"

int BulletPool::m_maxAmount = 15;
std::queue<EntityWrapper*> BulletPool::m_bulletsFreePool = {};

void BulletPool::SetActive(gce::GameObject* e, bool state)
{
	if(auto pc = e->GetComponent<PhysicComponent>())
		pc->SetActive(state);

	e->GetComponent<SphereCollider>()->SetActive(state);
	e->SetActive(state);
}

void BulletPool::Init()
{
	Geometry* pGeo = gce::SHAPES.SPHERE;

	for (size_t i = 0; i < m_maxAmount; i++)
	{
		EntityWrapper& bullet = EntityWrapper::Create();
		bullet.SetProperties("Bullet", { Tag::TProjectile, Tag::TBullet }, { 0, 0, 0 }, { 0, 0, 0 }, { 0.15, 0.15, 0.15 });
		bullet.AddMeshRenderer(pGeo, "");
		bullet.AddComponent<gce::SphereCollider>();
		bullet.AddScript<BulletBehavior>();

		SetActive(&bullet, false);
		m_bulletsFreePool.push(&bullet);
	}
}

EntityWrapper* BulletPool::Generate()
{
	if (m_bulletsFreePool.empty())
		return nullptr;

	EntityWrapper* pCurrent = m_bulletsFreePool.front();
	m_bulletsFreePool.pop();

	if(pCurrent->HasComponent<PhysicComponent>() == false)
		pCurrent->AddPhysics(0, 0, 0);

	SetActive(pCurrent, true);
	return pCurrent;
}

void BulletPool::Push(EntityWrapper* e)
{
	SetActive(e, false);
	m_bulletsFreePool.push(e);
}

void BulletPool::DesactivateAllBullets()
{
	for (gce::GameObject* go : GameManager::GetSceneManager().GetAllGameObjects({ Tag::TBullet }))
	{
		SetActive(go, false);
	}
}
