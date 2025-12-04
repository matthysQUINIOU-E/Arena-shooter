#include "Player.h"
#include "../SceneManager.h"
#include "Scripts/PlayerBehavior.hpp"

void Player::Create(gce::Scene& scene)
{
	m_pGameObject = &gce::GameObject::Create(scene);
	m_pGameObject->SetName("Player");
	m_pGameObject->transform.SetWorldScale({ 1, 2, 1 });
	m_pGameObject->transform.SetWorldPosition({ 0, 100, 0 });
	gce::PhysicComponent* pPhysic = m_pGameObject->AddComponent<gce::PhysicComponent>();

	gce::MeshRenderer* pPlayerMesh = m_pGameObject->AddComponent<gce::MeshRenderer>();
	pPlayerMesh->pGeometry = gce::SHAPES.CUBE;
	pPlayerMesh->pMaterial->albedoTextureID = GameManager::s_pInstance->m_pWhiteTexture->GetTextureID();
	pPlayerMesh->pMaterial->useTextureAlbedo = 1;
	pPlayerMesh->pMaterial->subsurface = 1;
	pPlayerMesh->pPso = gce::GameManager::GetSceneManager().GetPSO();

	gce::BoxCollider* bCollider = m_pGameObject->AddComponent<gce::BoxCollider>();

	PlayerBehavior* pScript = m_pGameObject->AddScript<PlayerBehavior>();
}

void Player::TestMusket(gce::Scene& scene)
{
	Texture* pMusket = new Texture("res/Assets/musket/musket_base_color.png");

	gce::GameObject& musket = gce::GameObject::Create(scene);
	musket.SetName("musket");

	gce::Vector3f32 playerPos = m_pGameObject->transform.GetWorldPosition();
	playerPos.z += -9.5;
	playerPos.y += -0.1;
	playerPos.x += 0.3f;

	musket.transform.SetWorldPosition(playerPos);
	//musket.transform.SetLocalPosition(playerPos);
	musket.transform.SetWorldScale({ 1, 1, 1 });
	musket.transform.SetWorldRotation({gce::PI, 0, gce::PI });
	gce::MeshRenderer* pMusketMesh = musket.AddComponent<gce::MeshRenderer>();
	pMusketMesh->pGeometry = gce::GeometryFactory::LoadGeometry("res/Assets/musket/musket.obj");
	pMusketMesh->pMaterial->albedoTextureID = pMusket->GetTextureID();
	pMusketMesh->pMaterial->useTextureAlbedo = 1;
	pMusketMesh->pMaterial->subsurface = 1;
	pMusketMesh->pPso = GameManager::GetSceneManager().GetPSO();

	m_pWeapons.push_back(&musket);

	gce::GameObject& hole = gce::GameObject::Create(scene);
	hole.SetName("hole");

	gce::Vector3f32 musketPos = musket.transform.GetWorldPosition();
	musketPos.z += 0.5;
	musketPos.y += 0.03;
	musketPos.x += 0;

	hole.transform.SetLocalPosition(musketPos);
	hole.transform.SetWorldScale({ 0.05, 0.05, 0.05 });
	gce::MeshRenderer* pHoleMesh = hole.AddComponent<gce::MeshRenderer>();
	pHoleMesh->pGeometry = SHAPES.SPHERE;
	pHoleMesh->pMaterial->albedoTextureID = GameManager::s_pInstance->m_pWhiteTexture->GetTextureID();
	pHoleMesh->pMaterial->useTextureAlbedo = 1;
	pHoleMesh->pMaterial->subsurface = 1;
	pHoleMesh->pPso = GameManager::GetSceneManager().GetPSO();
	musket.AddChild(hole);

	musket.AddScript<GunBehavior>();

	m_pGameObject->GetScript<PlayerBehavior>()->SetCurrentGun(&musket);
}

gce::GameObject* Player::GetWeapon(const char* name)
{
	for (gce::GameObject* go : m_pWeapons)
	{
		if (go->GetName() == name)
		{
			return go;
		}
	}

	return nullptr;
}
