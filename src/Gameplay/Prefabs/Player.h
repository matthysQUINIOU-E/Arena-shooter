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
public:
	void Create();

	EntityWrapper* GetGameObject() { return m_pGameObject; }
};

