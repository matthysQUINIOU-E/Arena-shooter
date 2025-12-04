#include "Player.h"
#include "../SceneManager.h"
#include "Scripts/PlayerBehavior.hpp"
#include "EntityWrapper.h"

void Player::Create()
{
	m_pGameObject = &EntityWrapper::Create();
	m_pGameObject->SetProperties("Player", Tag::TPlayer, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 2, 1 });

	m_pGameObject->AddPhysics(10, 1, 0);
	m_pGameObject->AddMeshRenderer(gce::SHAPES.CUBE, "");
	m_pGameObject->AddComponent<BoxCollider>();
	m_pGameObject->AddScript<PlayerBehavior>();
}

void Player::TestMusket()
{
	EntityWrapper& musket = EntityWrapper::Create();

	gce::Vector3f32 musketPos = m_pGameObject->transform.GetWorldPosition();
	musketPos.z += -9.5;
	musketPos.y += -0.1;
	musketPos.x += 0.3f;

	musket.SetProperties("Musket", Tag::TWeapon, musketPos, { gce::PI, 0, gce::PI }, { 1, 1, 1 });

	musket.AddMeshRenderer(gce::GeometryFactory::LoadGeometry("res/Assets/musket/musket.obj"), "res/Assets/musket/musket_base_color.png");

	m_pWeapons.push_back(&musket);

	EntityWrapper& hole = EntityWrapper::Create();

	gce::Vector3f32 holePos = musket.transform.GetWorldPosition();
	holePos.z += 0.5;
	holePos.y += 0.03;
	holePos.x += 0;

	hole.SetChildProperties(musket, "Musket Hole", Tag::TMiscellaneous, { 0, 0, 0 }, { 0, 0, 0 }, { 0.05, 0.05, 0.05 });
	hole.transform.SetWorldPosition(holePos);

	hole.AddMeshRenderer(gce::SHAPES.SPHERE, "");

	musket.AddScript<GunBehavior>();

	m_pGameObject->GetScript<PlayerBehavior>()->SetCurrentGun(&musket);
}

EntityWrapper* Player::GetWeapon(const char* name)
{
	for (EntityWrapper* go : m_pWeapons)
	{
		if (go->GetName() == name)
		{
			return go;
		}
	}

	return nullptr;
}
