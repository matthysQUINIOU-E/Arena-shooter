#include "StateMachinee.h"
#include "Agent.h"

class Actione
{
public:
	virtual void Update(float deltaTime) {}
};

class ReloadAmmo : public Actione
{
public:
	ReloadAmmo(int* ammo, int* maxAmmo, float baseReloadAmmo);
	void Update(float deltaTime) override;
private:
	int* m_ammo;
	int* m_maxAmmo;
	float m_baseReloadAmmo;
	float m_reloadAmmo;
};

class ChangeAgentRange : public Actione
{
public:
	ChangeAgentRange(Agent* agent, int* ammo, float meleeRange, float distanceRange);
	void Update(float deltaTime) override;
private:
	Agent* m_agent;
	int* m_ammo;
	float m_meleeRange;
	float m_distanceRange;
};