#pragma once
#include "GameObject.h"
#include <vector>

class Ammos;

class InventoryManager
{
	int m_inventoryIndex = 0;

	std::vector<gce::GameObject*> m_tmpWeapons; // GetWeapons before the gameobject pushback (only for the Init SceneManager)
	std::vector<gce::GameObject*> m_tmpCollectibles; // GetCollectibles before the gameobject pushback (only for the Init SceneManager)

	std::vector<gce::GameObject*> m_currentInventory;

	std::vector<Ammos*> m_ammoStock;

	gce::GameObject* m_pEquipedObject = nullptr;

	//Weapons Content
	gce::GameObject* CreateMusket();
	gce::GameObject* CreateBlunderBuss();

	//Collectibles Content
	gce::GameObject* CreateBomb();

public:
	~InventoryManager();

	void InitAll();

	std::vector<gce::GameObject*> GetWeapons();
	gce::GameObject* GetWeapon(SecondaryTag tag);
	std::vector<gce::GameObject*> GetCollectibles();
	gce::GameObject* GetCollectible(SecondaryTag tag);

	void UnInitTmp();

	void InitStates();

	void SwapEquipedObject(bool forward); // Forward = true to go on the next item in the Inventory, false to go backward
	void SetEquipedObjectByIndex(int index); // Equip instantly the new object with based on its location in the Inventory
	gce::GameObject* GetCurrentEquipedObject() { return m_pEquipedObject; }

	std::vector<gce::GameObject*>& GetCurrentInventory() { return m_currentInventory; }
	std::vector<Ammos*>& GetAmmoStock() { return m_ammoStock; }

	Ammos* GetAmmos(SecondaryTag type);
};

