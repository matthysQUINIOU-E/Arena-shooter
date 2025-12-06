#pragma once
#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>

using namespace gce;

DECLARE_SCRIPT(HealthBehavior, ScriptFlag::Start | ScriptFlag::Update)

int health = 1;
int maxHealth = 1;

const bool& IsAlive() const
{
	if (health <= 0)
		return false;

	return true;
}

void SetMaxHP(int val) { maxHealth = std::abs(val); health = maxHealth; }
void SetHP(int val)
{
	health = std::clamp(val, 0, maxHealth);
}

int TakeDamage(int val)
{
	health = std::clamp(health - val, 0, maxHealth);

	return val;
}
int Heal(int val)
{
	health = std::clamp(health + val, 0, maxHealth);

	return val;
}

void FillHP()
{
	health = maxHealth;
}

void Start()
{
}

void Update()
{
	/*if (IsAlive() == false)
	{
		std::cout << m_pOwner->GetName() << " is dead !\n";
	}
	else
	{
		std::cout << "PV : " << health << "/" << maxHealth << std::endl;
	}*/
}

END_SCRIPT