#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>

using namespace gce;

DECLARE_SCRIPT(AmmoManagerBehavior, ScriptFlag::Start | ScriptFlag::Update)

int ammos = 0;
int maxAmmos = 0;

const bool& HaveAmmos() const
{
	if (ammos <= 0)
		return false;

	return true;
}

const bool& IsFullAmmos() const
{
	if (ammos >= maxAmmos)
		return true;

	return false;
}

void SetMaxAmmos(int val) { maxAmmos = std::abs(val); ammos = maxAmmos; }
void SetAmmos(int val)
{
	ammos = std::clamp(val, 0, maxAmmos);
}

void UseAmmos(int val = 1)
{
	ammos = std::clamp(ammos - val, 0, maxAmmos);
}

void EarnAmmos(int val = 1)
{
	ammos = std::clamp(ammos + val, 0, maxAmmos);
}

void FillAmmos()
{
	ammos = maxAmmos;
}

void Start()
{
}

void Update()
{
	//if (HaveAmmos() == false)
	//{
	//	std::cout << m_pOwner->GetName() << " has no Ammo !\n";
	//}
	//else
	//{
	//	std::cout << "Ammos : " << ammos << "/" << maxAmmos << std::endl;
	//}
}

END_SCRIPT