#pragma once
#include"Actiones.h"

class DragonShoot : public Actione
{
	int* m_ammo;
	float m_CDtime;
	bool m_AttackReady = true;
	float m_BaseAttackCooldown;
	gce::GameObject* m_Dragon;
public:
	DragonShoot() {};
	DragonShoot(gce::GameObject* m_Dragon, int* ammo ,float CDtime);
	void Update(float deltaTime) override;
};
class DragonLaser : public Actione
{
	float m_ShootTime;
	bool m_AttackReady = true ;
	float m_LazerPlayerDistance;
	gce::GameObject* m_Dragon;

public:
	DragonLaser(gce::GameObject* Dragon , float ShootTime, float m_LazerPlayerDistance);

};

