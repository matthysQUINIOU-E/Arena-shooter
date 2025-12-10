#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include "Prefabs/ArenaCamera.h"
#include "Components.h"
#include "GunBehavior.hpp"
#include "../SceneManager.h"
#include "../Prefabs/InventoryManager.h"

using namespace gce;

DECLARE_SCRIPT(PlayerBehavior, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::CollisionStay | ScriptFlag::CollisionExit | ScriptFlag::Destroy)

//Members

PhysicComponent* pPhysic = nullptr;

GameObject* pWeapon = nullptr;

float speed = 5.f;
int jumpsAmount = 0;
int maxJumpsAmount = 2;

bool isJumping = false;

float sensitivity = 0.002f;
gce::Vector2i32 middleScreen = { (int)((float)(WINDOW_WIDTH) * 0.5f), (int)((float)(WINDOW_HEIGHT) * 0.5f) };
float totalPitchRotation = 0.f;

void LookAround()
{
	if (m_pOwner->GetChildren().Empty())
	{
		ShowMouseCursor();
		return;
	}

	gce::GameObject* pCamera = m_pOwner->GetChildren()[0];
	// Delta Mouse Calcul
	HideMouseCursor();

	POINT currentMousePos;
	GetCursorPos(&currentMousePos);
	gce::Vector2i32 mouseDelta = { (int)(currentMousePos.x - middleScreen.x), (int)(currentMousePos.y - middleScreen.y) };

	float yaw = mouseDelta.x * sensitivity;
	float pitch = mouseDelta.y * sensitivity;

	//Don't allow to look the world upside down (e.g more than 90 degrees toward up)
	totalPitchRotation += pitch;
	totalPitchRotation = std::clamp(totalPitchRotation, -gce::PI / 2, gce::PI / 2);

	// Set Rotation for Player
	m_pOwner->transform.WorldRotate({ 0.f, yaw, 0.f });

	Quaternion pitchQ = Quaternion::RotationEuler({ totalPitchRotation, 0.f, 0.f });

	pCamera->transform.SetLocalRotation(pitchQ);

	SetCursorPos(middleScreen.x, middleScreen.y);
}

void BasicControls() // Move + Jump
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

	m_pOwner->transform.WorldTranslate((finalDir * speed * dt));
}

//Functions
void HandleInput()
{
	if (pPhysic == nullptr)
		return;

	BasicControls();
	
	// Shoot
	if (pWeapon != nullptr)
	{
		if (GetButton(Mouse::LEFT))
		{
			pWeapon->GetScript<GunBehavior>()->Shoot();
		}
	}

	// Swap Weapon
	if (GetKeyDown(Keyboard::_1))
	{
		GameManager::GetSceneManager().GetInventoryManager()->SetEquipedObjectByIndex(0);
	}
	else if (GetKeyDown(Keyboard::_2))
	{
		GameManager::GetSceneManager().GetInventoryManager()->SetEquipedObjectByIndex(1);
	}

}

void SetCurrentWeapon(GameObject* go) { pWeapon = go; }

void Start()
{
	pPhysic = m_pOwner->GetComponent<PhysicComponent>();

	if (pPhysic == nullptr)
		return;

	pPhysic->SetBounciness(0.f);
}

void Update()
{
	SetCurrentWeapon(GameManager::GetSceneManager().GetInventoryManager()->GetCurrentEquipedObject());

	HandleInput();
	LookAround();
}

void Destroy()
{
	Console::Log("[TestScript1] Destroy has been called.");
}

void CollisionStay(GameObject* other)
{
	if (true) // TO DO Set Tag for the loaded Scene
	{ 
		if (isJumping == false)
		{
			jumpsAmount = maxJumpsAmount;
		}
	}
}

void CollisionEnter(GameObject* other)
{
	if (true) // TO DO Set Tag for the loaded Scene
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
	if (other->GetGlobalTag() == PrimaryTag::TGround && isJumping == false)
		jumpsAmount = 0;
}

END_SCRIPT
