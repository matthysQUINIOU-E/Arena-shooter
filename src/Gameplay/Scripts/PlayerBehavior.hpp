#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include "Prefabs/ArenaCamera.h"
#include "Components.h"
#include "GunBehavior.hpp"

using namespace gce;

DECLARE_SCRIPT(PlayerBehavior, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::CollisionStay | ScriptFlag::CollisionExit | ScriptFlag::Destroy)

//Members
PhysicComponent* pPhysic = nullptr;

GameObject* pGun = nullptr;

float speed = 5.f;
int jumpsAmount = 0;
int maxJumpsAmount = 2;

bool isJumping = false;

//Functions
void HandleInput()
{
	if (pPhysic == nullptr)
		return;

	gce::Vector3f32 velocity = pPhysic->GetVelocity();
	pPhysic->SetVelocity({ 0, velocity.y, 0 });

	float dt = GameManager::DeltaTime();

	gce::Vector3f32 dir = {};

	if (GetKey(Keyboard::Q))
		dir.x -= 1;
	if (GetKey(Keyboard::D))
		dir.x += 1;
	if (GetKey(Keyboard::Z))
		dir.z += 1;
	if (GetKey(Keyboard::S))
		dir.z -= 1;

	dir.SelfNormalize();

	gce::Vector3f32 finalDir = m_pOwner->transform.GetWorldForward() * dir.z + m_pOwner->transform.GetWorldRight() * dir.x; // Redirect Direction By Rotation

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

	m_pOwner->transform.LocalTranslate((finalDir * speed * dt));

	if (pGun != nullptr)
	{
		if (GetButtonDown(Mouse::LEFT))
		{
			pGun->GetScript<GunBehavior>()->Shoot();
		}
	}
}

void SetCurrentGun(GameObject* go) { pGun = go; }

void Start()
{
	pPhysic = m_pOwner->GetComponent<PhysicComponent>();
	pPhysic->SetBounciness(0.f);
}

void Update()
{
	HandleInput();
}

void Destroy()
{
	Console::Log("[TestScript1] Destroy has been called.");
}

void CollisionStay(GameObject* other)
{
	if (other->GetTag() == Tag::TGround)
	{
		if (isJumping == false)
		{
			jumpsAmount = maxJumpsAmount;
		}
	}
}

void CollisionEnter(GameObject* other)
{
	if (other->GetTag() == Tag::TGround)
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
	if (other->GetTag() == Tag::TGround && isJumping == false)
		jumpsAmount = 0;
}

END_SCRIPT
