#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include "Prefabs/ArenaCamera.h"
#include "Components.h"

using namespace gce;

DECLARE_SCRIPT(PlayerBehavior, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::CollisionStay | ScriptFlag::CollisionExit | ScriptFlag::Destroy)

//Members
ArenaCamera* pCam = nullptr;
PhysicComponent* pPhysic = nullptr;

int jumpsAmount = 0;
int maxJumpsAmount = 2;

bool isJumping = false;

//Functions
void SetCam(ArenaCamera* pCamera)
{
	pCam = pCamera;

	pPhysic = m_pOwner->GetComponent<PhysicComponent>();
	pPhysic->SetBounciness(0.f);
}

void HandleInput()
{
	if (pPhysic == nullptr)
		return;

	gce::Vector3f32 velocity = pPhysic->GetVelocity();
	pPhysic->SetVelocity({ 0, velocity.y, 0 });

	float dt = GameManager::DeltaTime();
	float speed = 2.f;

	gce::Vector3f32 dir = {};

	if (GetKey(Keyboard::Q))
		dir.x -= 1;
	if (GetKey(Keyboard::D))
		dir.x += 1;
	if (GetKey(Keyboard::Z))
		dir.z += 1;
	if (GetKey(Keyboard::S))
		dir.z -= 1;

	if (GetKeyDown(Keyboard::SPACE))
	{
		if (jumpsAmount > 0)
		{
			isJumping = true;
			jumpsAmount--;

			gce::Force f;

			f.direction = { 0, 1, 0 };
			f.norm = 15000;
			f.useApplicationPoint = true;
			f.relativeApplicationPoint = { 0, 0, 0 };
			pPhysic->SetVelocity({ velocity.x, 0, velocity.z });

			pPhysic->AddForce(f);
		}
	}

	dir.SelfNormalize();

	m_pOwner->transform.WorldTranslate((dir * speed * dt));
}


void Start()
{
	m_pOwner->transform.SetWorldPosition({ 0, 0, 0 });
}

void Update()
{
	if (pCam != nullptr)
	{
		gce::Vector3f32 newPos = m_pOwner->transform.GetWorldPosition();
		newPos.z -= 10.f;

		pCam->GetGameObject()->transform.SetWorldPosition(newPos);
	}

	HandleInput();
	std::cout << jumpsAmount << std::endl;
}

void Destroy()
{
	Console::Log("[TestScript1] Destroy has been called.");
}

void CollisionStay(GameObject* other)
{
	if (other->GetName() == "Floor")
	{
		if (isJumping == false)
		{
			jumpsAmount = maxJumpsAmount;
		}
	}
}

void CollisionEnter(GameObject* other)
{
	if (other->GetName() == "Floor")
	{
		if (isJumping)
		{
			isJumping = false;
		}
		else
		{
			jumpsAmount = maxJumpsAmount;
		}
	}
}

void CollisionExit(GameObject* other) override
{
	if (other->GetName() == "Floor" && isJumping == false)
		jumpsAmount = 0;
}

END_SCRIPT
