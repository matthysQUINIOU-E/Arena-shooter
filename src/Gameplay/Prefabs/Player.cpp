#include "Player.h"
#include "../SceneManager.h"
#include "Scripts/PlayerBehavior.hpp"
#include "EntityWrapper.h"

void Player::Create()
{
	m_pGameObject = &EntityWrapper::Create();
	m_pGameObject->SetProperties("Player", Tag1::TPlayer, Tag2::None, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 2, 1 });

	m_pGameObject->AddPhysics(10, 1, 0);
	m_pGameObject->AddMeshRenderer(gce::SHAPES.CUBE, "");
	m_pGameObject->AddComponent<BoxCollider>();
	m_pGameObject->AddScript<PlayerBehavior>();
}
