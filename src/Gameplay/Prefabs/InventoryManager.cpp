#include "pch.h"
#include "InventoryManager.h"
#include "EntityWrapper.h"

#include "Scripts/GunBehavior.hpp"
#include "Ammos.h"

InventoryManager::~InventoryManager()
{
	for (Ammos* a : m_ammoStock)
	{
		delete a;
	}

	m_ammoStock.clear();
}

//Weapons Content
gce::GameObject* InventoryManager::CreateMusket()
{
	EntityWrapper& musket = EntityWrapper::Create();

	musket.SetProperties("Musket", { Tag::TWeapon, Tag::TMusket }, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
	m_pSceneManager->GetCameraObject()->AddChild(musket);

	musket.transform.SetLocalPosition({ 0.25, -0.1f, 0.5f });

	musket.AddMeshRenderer(gce::GeometryFactory::LoadGeometry("res/Assets/musket/musket.obj"), "res/Assets/musket/musket_base_color.png");
	EntityWrapper& hole = EntityWrapper::Create();
	gce::Vector3f32 holePos = {};
	holePos.z -= 0.5;
	holePos.y += 0.03;

	hole.SetChildProperties(musket, "Musket Hole", { Tag::TMiscellaneous }, { 0, 0, 0 }, { 0, 0, 0 }, { 0.05, 0.05, 0.05 });
	hole.transform.LocalTranslate(holePos);

	auto ammoManagerScript = musket.AddScript<WeaponMagazineBehavior>();
	ammoManagerScript->SetMaxCapacity(30);

	auto gunBehavior = musket.AddScript<GunBehavior>();
	gunBehavior->SetUnloadSpeed(0.2);
	gunBehavior->SetReloadTime(1.f);
	gunBehavior->SetAmmoManagerScript(ammoManagerScript);

	m_pSceneManager->LinkObjectToScene(&musket, SceneType::GamePlayScene);

	return &musket;
}

gce::GameObject* InventoryManager::CreateBlunderBuss()
{
	EntityWrapper& blunderbuss = EntityWrapper::Create();

	blunderbuss.SetProperties("Blunderbuss", { Tag::TWeapon, Tag::TBlunderBuss }, { 0, 0, 0.f }, { 0, 0, 0 }, { 1, 1, 1 });
	m_pSceneManager->GetCameraObject()->AddChild(blunderbuss);

	blunderbuss.transform.SetLocalPosition({ 0.25, -0.1f, 0.5f });

	blunderbuss.AddMeshRenderer(gce::GeometryFactory::LoadGeometry("res/Assets/blunderbuss/blunderbuss.obj"), "res/Assets/blunderbuss/blunderbuss_base_color.png");

	EntityWrapper& hole = EntityWrapper::Create();
	gce::Vector3f32 holePos = {};
	holePos.z -= 0.9;
	holePos.y += 0;

	hole.SetChildProperties(blunderbuss, "Blunderbuss Hole", { Tag::TMiscellaneous }, { 0, 0, 0 }, { 0, 0, 0 }, { 0.1, 0.1, 0.1 });
	hole.transform.LocalTranslate(holePos);

	auto ammoManagerScript = blunderbuss.AddScript<WeaponMagazineBehavior>();
	ammoManagerScript->SetMaxCapacity(8);

	auto gunBehavior = blunderbuss.AddScript<GunBehavior>();
	gunBehavior->SetUnloadSpeed(0.5);
	gunBehavior->SetReloadTime(2);
	gunBehavior->SetAmmoManagerScript(ammoManagerScript);

	m_pSceneManager->LinkObjectToScene(&blunderbuss, SceneType::GamePlayScene);

	return &blunderbuss;
}

//Collectibles Content
gce::GameObject* InventoryManager::CreateBomb()
{
	EntityWrapper& bomb = EntityWrapper::Create();
	m_pSceneManager->GetCameraObject()->AddChild(bomb);

	bomb.SetProperties("Bomb", { Tag::TThrowableWeapon, Tag::TBomb }, { 0, 0, 0 }, { 0, 0, 0 }, { 2, 2, 2 });
	bomb.transform.SetLocalPosition({ 0.25, -0.1f, 0.5f });

	bomb.AddMeshRenderer(gce::GeometryFactory::LoadGeometry("res/Assets/bomb/bomb.obj"), "res/Assets/bomb/bomb_base_color.png");

	EntityWrapper& hole = EntityWrapper::Create();
	gce::Vector3f32 holePos = bomb.transform.GetWorldPosition();

	hole.SetChildProperties(bomb, "Bomb Hole", { Tag::TMiscellaneous }, { 0, 0, 0 }, { 0, 0, 0 }, { 0.05, 0.05, 0.05 });
	hole.transform.SetWorldPosition(holePos);

	m_pSceneManager->LinkObjectToScene(&bomb, SceneType::GamePlayScene);

	return &bomb;
}

void InventoryManager::InitAll()
{
	m_pSceneManager = &GameManager::GetSceneManager();

	m_tmpWeapons.push_back(CreateMusket());
	m_tmpWeapons.push_back(CreateBlunderBuss());
	
	//m_tmpCollectibles.push_back(CreateBomb());

	m_currentInventory.push_back(GetWeapon(Tag::TBlunderBuss));
	m_currentInventory.push_back(GetWeapon(Tag::TMusket));

	// AMMMOS
	m_ammoStock.push_back(new Ammos(Tag::THeavyAmmo, 30));
	m_ammoStock.push_back(new Ammos(Tag::TNormalAmmo, 100));
	m_ammoStock.push_back(new Ammos(Tag::TLightAmmo, 200));
}

void InventoryManager::ResetAll()
{
	m_inventoryIndex = 0;
	m_pEquipedObject = nullptr;

	UnInitTmp();
	m_currentInventory.clear();

	for (Ammos* ammos : m_ammoStock)
	{
		delete ammos;
	}

	m_ammoStock.clear();
}

std::vector<gce::GameObject*> InventoryManager::GetWeapons()
{
	if (m_tmpWeapons.empty() == false)
	{
		std::vector<gce::GameObject*> weapons = m_tmpWeapons;

		return weapons;
	}

	return gce::GameManager::GetSceneManager().GetAllGameObjects({ Tag::TWeapon });
}

gce::GameObject* InventoryManager::GetWeapon(Tag tag)
{
	if(m_tmpWeapons.empty() == false)
	{
		for (gce::GameObject* go : m_tmpWeapons)
		{
			if (go->IsTags({ tag }))
				return go;
		}
	}

	return gce::GameManager::GetSceneManager().GetFirstGameObject({ Tag::TWeapon, tag });
}

std::vector<gce::GameObject*> InventoryManager::GetCollectibles()
{
	if (m_tmpCollectibles.empty() == false)
	{
		std::vector<gce::GameObject*> collectibles = m_tmpCollectibles;

		return collectibles;
	}

	return gce::GameManager::GetSceneManager().GetAllGameObjects({ Tag::TThrowableWeapon });
}

gce::GameObject* InventoryManager::GetCollectible(Tag tag)
{
	if (m_tmpCollectibles.empty() == false)
	{
		for (gce::GameObject* go : m_tmpCollectibles)
		{
			if (go->IsTags({ tag }))
				return go;
		}
	}

	return gce::GameManager::GetSceneManager().GetFirstGameObject({ Tag::TThrowableWeapon, tag });
}

void InventoryManager::UnInitTmp()
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

	if (auto current = m_currentInventory[m_inventoryIndex])
	{
		if (current->IsTags({ Tag::TWeapon }))
			current->GetScript<GunBehavior>()->OnLeaveWeapon(); // The old weapon
	}

	m_inventoryIndex += sens;

	gce::GameObject* newEquipedObject = m_currentInventory[m_inventoryIndex];

	for (gce::GameObject* pObject : m_currentInventory)
	{
		//Only the new Equiped Object is active

		if (pObject == newEquipedObject) // if the current checked Object among the inventory is the new equiped
		{
			pObject->SetActive(true);
			m_pEquipedObject = pObject;

			if (pObject->IsTags({ Tag::TWeapon }))
			{
				pObject->GetScript<GunBehavior>()->OnReceiveWeapon();
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

	if (auto current = m_currentInventory[m_inventoryIndex])
	{
		if (current->IsTags({ Tag::TWeapon }))
			current->GetScript<GunBehavior>()->OnLeaveWeapon(); // The old weapon
	}

	m_inventoryIndex = index;

	gce::GameObject* newEquipedObject = m_currentInventory[m_inventoryIndex];

	for (gce::GameObject* pObject : m_currentInventory)
	{
		//Only the new Equiped Object is active

		if (pObject == newEquipedObject) // if the current checked Object among the inventory is the new equiped
		{
			pObject->SetActive(true);
			m_pEquipedObject = pObject;

			if (pObject->IsTags({ Tag::TWeapon }))
			{
				pObject->GetScript<GunBehavior>()->OnReceiveWeapon();
			}
		}
		else
		{
			pObject->SetActive(false);
		}
	}
}

Ammos* InventoryManager::GetAmmos(Tag type)
{
	for (Ammos* a : m_ammoStock)
	{
		if (a->IsType(type))
			return a;
	}

	return nullptr;
}
