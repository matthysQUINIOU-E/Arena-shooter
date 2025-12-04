#pragma once

#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include "BulletBehavior.hpp"

using namespace gce;

DECLARE_SCRIPT(GunBehavior, ScriptFlag::Start | ScriptFlag::Update)

void Shoot()
{
	Scene& scene = GameManager::GetScene();

	GameObject& bullet = GameObject::Create(scene);

	if (m_pOwner->GetChildren().Empty())
	{
		return;
	}

	bullet.transform.SetWorldPosition(m_pOwner->GetChildren()[0]->transform.GetWorldPosition());

	auto& angles = m_pOwner->transform.GetWorldRotation();

	bullet.transform.SetWorldRotation(angles);
	bullet.transform.SetWorldScale({ 0.15, 0.15, 0.15 });

	MeshRenderer* pMesh = bullet.AddComponent<MeshRenderer>();
	pMesh->pGeometry = gce::SHAPES.SPHERE;
	pMesh->pMaterial->albedoTextureID = GameManager::s_pInstance->m_pWhiteTexture->GetTextureID();
	pMesh->pMaterial->useTextureAlbedo = 1;
	pMesh->pMaterial->subsurface = 1;
	pMesh->pPso = gce::GameManager::GetSceneManager().GetPSO();

	/*bullet.AddComponent<PhysicComponent>();*/
	bullet.AddComponent<SphereCollider>();
	bullet.AddScript<BulletBehavior>();
}

void Start()
{
}

void Update()
{
	
}

END_SCRIPT