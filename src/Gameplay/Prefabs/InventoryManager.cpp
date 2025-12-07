#include "pch.h"
#include "InventoryManager.h"
#include "EntityWrapper.h"

#include "Scripts/GunBehavior.hpp"

//Weapons Content
gce::GameObject* InventoryManager::CreateMusket()
{
	EntityWrapper& musket = EntityWrapper::Create();

	musket.SetProperties("Musket", Tag1::TWeapon, Tag2::TMusket, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
	musket.transform.SetLocalPosition({ 0.25, -0.1f, 0.5f });

	musket.AddMeshRenderer(gce::GeometryFactory::LoadGeometry("res/Assets/musket/musket.obj"), "res/Assets/musket/musket_base_color.png");
	EntityWrapper& hole = EntityWrapper::Create();
	gce::Vector3f32 holePos = {};
	holePos.z -= 0.5;
	holePos.y += 0.03;

	hole.SetChildProperties(musket, "Musket Hole", Tag1::TMiscellaneous, Tag2::None, {0, 0, 0}, {0, 0, 0}, {0.05, 0.05, 0.05});
	hole.transform.LocalTranslate(holePos);

	auto ammoManagerScript = musket.AddScript<AmmoManagerBehavior>();
	ammoManagerScript->SetMaxAmmos(12);

	auto gunBehavior = musket.AddScript<GunBehavior>();
	gunBehavior->SetUnloadSpeed(0.1);
	gunBehavior->SetReloadTime(1.f);
	gunBehavior->SetAmmoManagerScript(ammoManagerScript);

	return &musket;
}

gce::GameObject* InventoryManager::CreateBlunderBuss()
{
	EntityWrapper& blunderbuss = EntityWrapper::Create();

	blunderbuss.SetProperties("Blunderbuss", Tag1::TWeapon, Tag2::TBlunderBuss, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
	blunderbuss.transform.SetLocalPosition({ 0.25, -0.1f, 0.5f });
	
	blunderbuss.AddMeshRenderer(gce::GeometryFactory::LoadGeometry("res/Assets/blunderbuss/blunderbuss.obj"), "res/Assets/blunderbuss/blunderbuss_base_color.png");

	EntityWrapper& hole = EntityWrapper::Create();
	gce::Vector3f32 holePos = {};
	holePos.z -= 1;
	holePos.y += 0;

	hole.SetChildProperties(blunderbuss, "Blunderbuss Hole", Tag1::TMiscellaneous, Tag2::None, { 0, 0, 0 }, { 0, 0, 0 }, { 0.1, 0.1, 0.1 });
	hole.transform.LocalTranslate(holePos);

	auto ammoManagerScript = blunderbuss.AddScript<AmmoManagerBehavior>();
	ammoManagerScript->SetMaxAmmos(8);

	auto gunBehavior = blunderbuss.AddScript<GunBehavior>();
	gunBehavior->SetUnloadSpeed(0.5);
	gunBehavior->SetReloadTime(1.2);
	gunBehavior->SetAmmoManagerScript(ammoManagerScript);

	return &blunderbuss;
}

//Collectibles Content
gce::GameObject* InventoryManager::CreateBomb()
{
	EntityWrapper& bomb = EntityWrapper::Create();

	bomb.SetProperties("Bomb", Tag1::TThrowableWeapon, Tag2::TBomb, { 0, 0, 0 }, { 0, 0, 0 }, { 2, 2, 2 });
	bomb.transform.SetLocalPosition({ 0.25, -0.1f, 0.5f });

	bomb.AddMeshRenderer(gce::GeometryFactory::LoadGeometry("res/Assets/bomb/bomb.obj"), "res/Assets/bomb/bomb_base_color.png");

	EntityWrapper& hole = EntityWrapper::Create();
	gce::Vector3f32 holePos = bomb.transform.GetWorldPosition();

	hole.SetChildProperties(bomb, "Bomb Hole", Tag1::TMiscellaneous, Tag2::None, { 0, 0, 0 }, { 0, 0, 0 }, { 0.05, 0.05, 0.05 });
	hole.transform.SetWorldPosition(holePos);

	return &bomb;
}

void InventoryManager::InitAll()
{
	m_tmpWeapons.push_back(CreateMusket());
	m_tmpWeapons.push_back(CreateBlunderBuss());
	
	//m_tmpCollectibles.push_back(CreateBomb());

	m_currentInventory.push_back(GetWeapon(Tag2::TBlunderBuss));
	m_currentInventory.push_back(GetWeapon(Tag2::TMusket));
}

std::vector<gce::GameObject*> InventoryManager::GetWeapons()
{
	if (m_tmpWeapons.empty() == false)
	{
		std::vector<gce::GameObject*> weapons = m_tmpWeapons;

		return weapons;
	}

	return gce::GameManager::GetSceneManager().GetAllGameObjects(Tag1::TWeapon);
}

gce::GameObject* InventoryManager::GetWeapon(Tag2 tag)
{
	if(m_tmpWeapons.empty() == false)
	{
		for (gce::GameObject* go : m_tmpWeapons)
		{
			if (go->IsTag2(tag))
				return go;
		}
	}

	return gce::GameManager::GetSceneManager().GetFirstGameObject(Tag1::TWeapon, tag);
}

std::vector<gce::GameObject*> InventoryManager::GetCollectibles()
{
	if (m_tmpCollectibles.empty() == false)
	{
		std::vector<gce::GameObject*> collectibles = m_tmpCollectibles;

		return collectibles;
	}

	return gce::GameManager::GetSceneManager().GetAllGameObjects(Tag1::TThrowableWeapon);
}

gce::GameObject* InventoryManager::GetCollectible(Tag2 tag)
{
	if (m_tmpCollectibles.empty() == false)
	{
		for (gce::GameObject* go : m_tmpCollectibles)
		{
			if (go->IsTag2(tag))
				return go;
		}
	}

	return gce::GameManager::GetSceneManager().GetFirstGameObject(Tag1::TThrowableWeapon, tag);
}

void InventoryManager::UnInitAll()
{
	m_tmpWeapons.clear();
	m_tmpCollectibles.clear();
}

void InventoryManager::InitStates()
{
	for (gce::GameObject* pObject : m_currentInventory)
	{
		//Only the new Equiped Object is active
		gce::GameObject* first = m_currentInventory[0];

		if (pObject == first) // if the current checked Object among the inventory is the new equiped
		{
			pObject->SetActive(true);
			m_pEquipedObject = pObject;
		}
		else
		{
			pObject->SetActive(false);
		}
	}
}

void InventoryManager::SwapEquipedObject(bool forward)
{
	int sens = forward ? 1 : -1;

	if (m_inventoryIndex + sens < 0)
		return;

	if (m_inventoryIndex + sens > m_currentInventory.size() - 1)
		return;

	m_inventoryIndex += sens;

	gce::GameObject* newEquipedObject = m_currentInventory[m_inventoryIndex];

	for (gce::GameObject* pObject : m_currentInventory)
	{
		//Only the new Equiped Object is active

		if (pObject == newEquipedObject) // if the current checked Object among the inventory is the new equiped
		{
			pObject->SetActive(true);
			m_pEquipedObject = pObject;

			if (pObject->IsTag1(Tag1::TWeapon))
			{
				pObject->GetScript<GunBehavior>()->HandleSwapGun();
			}
		}
		else
		{
			pObject->SetActive(false);
		}
	}
}

void InventoryManager::SetEquipedObjectByIndex(int index)
{
	if (index == m_inventoryIndex) // already equiped
		return;

	if (index < 0) // outrange
		return;

	if (index > m_currentInventory.size() - 1) // outrange
		return;

	m_inventoryIndex = index;

	gce::GameObject* newEquipedObject = m_currentInventory[m_inventoryIndex];

	for (gce::GameObject* pObject : m_currentInventory)
	{
		//Only the new Equiped Object is active

		if (pObject == newEquipedObject) // if the current checked Object among the inventory is the new equiped
		{
			pObject->SetActive(true);
			m_pEquipedObject = pObject;

			if (pObject->IsTag1(Tag1::TWeapon))
			{
				pObject->GetScript<GunBehavior>()->HandleSwapGun();
			}
		}
		else
		{
			pObject->SetActive(false);
		}
	}
}
