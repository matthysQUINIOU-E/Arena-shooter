#include "Player.h"
#include "../SceneManager.h"
#include "Scripts/PlayerBehavior.hpp"
#include "EntityWrapper.h"
#include "../Scripts/HealthBehavior.hpp"

void Player::Create()
{
	m_pGameObject = &EntityWrapper::Create();
	m_pGameObject->SetProperties("Player", { Tag::TPlayer }, { 10, 2, 0 });

	m_pGameObject->AddPhysics(10, 1, 0);
	m_pGameObject->AddMeshRenderer(gce::SHAPES.CYLINDER, "");

	m_pGameObject->AddComponent<BoxCollider>();
	m_pGameObject->AddScript<PlayerBehavior>();

	auto health = m_pGameObject->AddScript<HealthBehavior>();
	health->SetMaxHP(100000);
}
