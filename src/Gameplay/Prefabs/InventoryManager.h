#pragma once
#include "GameObject.h"
#include <vector>

class InventoryManager
{
	int m_inventoryIndex = 0;

	std::vector<gce::GameObject*> m_tmpWeapons; // GetWeapons before the gameobject pushback (only for the Init SceneManager)
	std::vector<gce::GameObject*> m_tmpCollectibles; // GetCollectibles before the gameobject pushback (only for the Init SceneManager)

	std::vector<gce::GameObject*> m_currentInventory;

	gce::GameObject* m_pEquipedObject = nullptr;

	//Weapons Content
	gce::GameObject* CreateMusket();
	gce::GameObject* CreateBlunderBuss();

	//Collectibles Content
	gce::GameObject* CreateBomb();

public:
	void InitAll();

	std::vector<gce::GameObject*> GetWeapons();
	gce::GameObject* GetWeapon(Tag2 tag);
	std::vector<gce::GameObject*> GetCollectibles();
	gce::GameObject* GetCollectible(Tag2 tag);

	void UnInitAll();

	void InitStates();

	void SwapEquipedObject(bool forward); // Forward = true to go on the next item in the Inventory, false to go backward
	void SetEquipedObjectByIndex(int index); // Equip instantly the new object with based on its location in the Inventory
	gce::GameObject* GetCurrentEquipedObject() { return m_pEquipedObject; }

	std::vector<gce::GameObject*>& GetCurrentInventory() { return m_currentInventory; }
};

