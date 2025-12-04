#pragma once
#include "GameObject.h"

namespace gce
{
	class Texture;
}

class Player
{
	gce::GameObject* m_pGameObject = nullptr;

	std::vector<gce::GameObject*> m_pWeapons;
public:
	void Create(gce::Scene& scene);

	void TestMusket(gce::Scene& scene);

	gce::GameObject* GetWeapon(const char* name);

	gce::GameObject* GetGameObject() { return m_pGameObject; }
};

