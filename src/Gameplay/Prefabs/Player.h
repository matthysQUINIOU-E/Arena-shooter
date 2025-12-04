#pragma once
#include "GameObject.h"

class EntityWrapper;

namespace gce
{
	class Texture;
}

class Player
{
	EntityWrapper* m_pGameObject;

	std::vector<EntityWrapper*> m_pWeapons;
public:
	void Create();

	void TestMusket();

	EntityWrapper* GetWeapon(const char* name);

	EntityWrapper* GetGameObject() { return m_pGameObject; }
};

