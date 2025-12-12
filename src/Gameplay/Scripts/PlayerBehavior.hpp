#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include "Prefabs/ArenaCamera.h"
#include "Components.h"
#include "GunBehavior.hpp"
#include "../SceneManager.h"
#include "../Prefabs/InventoryManager.h"
#include "HealthBehavior.hpp"
#include "KeyBinds.h"

using namespace gce;

DECLARE_SCRIPT(PlayerBehavior, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::CollisionStay | ScriptFlag::CollisionExit | ScriptFlag::Destroy)

//Members /////////////////////////
PhysicComponent* pPhysic = nullptr;
GameObject* pWeapon = nullptr;

float playerSpeed = 5.f;
gce::Vector3f32 finalDir = {};

//Jump
bool isJumping = false;
int jumpsAmount = 0;
int maxJumpsAmount = 2;

//Dash
bool isDashing = false;
float maxDashAmount = 2;
float dashAmount = 0.f;
float dashTotalReloadTime = 6.f;
float dashProgressReloadTime = 0.f;

float dashDuration = 0.1f;
float dashProgressDuration = 0.f;

//Camera
float sensitivity = 0.002f;
gce::Vector2i32 middleScreen = { (int)((float)(WINDOW_WIDTH) * 0.5f), (int)((float)(WINDOW_HEIGHT) * 0.5f) };
float totalPitchRotation = 0.f;
bool stopLookAround = false;

//Functions
void LookAround()
{
	if (m_pOwner->GetChildren().Empty() || stopLookAround == true)
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

	if (isDashing == false)
	{
		gce::Vector3f32 velocity = pPhysic->GetVelocity();
		pPhysic->SetVelocity({ 0, velocity.y, 0 });
	}

	float dt = GameManager::DeltaTime();

	gce::Vector3f32 dir = {};

	if (GetKey(KeyBinds::GetKeyBind(KeyAction::MoveLeft)))
		dir.x -= 1;
	if (GetKey(KeyBinds::GetKeyBind(KeyAction::MoveRight)))
		dir.x += 1;
	if (GetKey(KeyBinds::GetKeyBind(KeyAction::MoveForward)))
		dir.z += 1;
	if (GetKey(KeyBinds::GetKeyBind(KeyAction::MoveBackward)))
		dir.z -= 1;

	dir.SelfNormalize();

	finalDir = m_pOwner->transform.GetWorldForward() * dir.z + m_pOwner->transform.GetWorldRight() * dir.x; // Redirect Direction By Rotation

	if (GetKeyDown(KeyBinds::GetKeyBind(KeyAction::Jump)))
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

			pPhysic->AddForce(f);
		}
	}

	m_pOwner->transform.WorldTranslate((finalDir * playerSpeed * dt));
}
void HandleDash()
{
	float dt = GameManager::DeltaTime();

	if (isDashing)
	{
		if (dashProgressDuration < dashDuration)
		{
			dashProgressDuration += dt;

			gce::Force f;
			f.direction = finalDir;
			f.norm = 22750;
			f.useApplicationPoint = true;
			f.relativeApplicationPoint = { 0, 0, 0 };

			pPhysic->AddForce(f);
		}
		else
		{
			dashProgressDuration = 0.f;
			isDashing = false;
		}

		return;
	}

	if (dashProgressReloadTime < dashTotalReloadTime)
	{
		dashProgressReloadTime += dt;
	}

	dashAmount = (int)(maxDashAmount * dashProgressReloadTime / dashTotalReloadTime);

	if (dashAmount > 0)
	{
		if (GetKeyDown(KeyBinds::GetKeyBind(KeyAction::Dash)))
		{
			pPhysic->SetVelocity({ 0, 0, 0 });

			dashProgressReloadTime -= dashTotalReloadTime / (float)maxDashAmount;
			isDashing = true;
		}
	}
}

void HandleInput()
{
	if (pPhysic == nullptr)
		return;

	BasicControls();
	HandleDash();

	// Shoot
	if (pWeapon != nullptr)
	{
		GunBehavior* gunScript = pWeapon->GetScript<GunBehavior>();

		if (gunScript->IsReadyToUse())
		{
			if (GetButton(Mouse::LEFT))
				gunScript->Shoot();

			if (GetKeyDown(KeyBinds::GetKeyBind(KeyAction::Reload)))
				gunScript->TriggerReload();
		}
	}

	// Swap Weapon
	if (GetKeyDown(KeyBinds::GetKeyBind(KeyAction::InventorySlot1)))
	{
		GameManager::GetSceneManager().GetInventoryManager()->SetEquipedObjectByIndex(0);
	}
	else if (GetKeyDown(KeyBinds::GetKeyBind(KeyAction::InventorySlot2)))
	{
		GameManager::GetSceneManager().GetInventoryManager()->SetEquipedObjectByIndex(1);
	}
	else if (GetKeyDown(KeyBinds::GetKeyBind(KeyAction::InventorySlot3)))
	{
		GameManager::GetSceneManager().GetInventoryManager()->SetEquipedObjectByIndex(2);
	}
}

void SetCurrentWeapon(GameObject* go) { pWeapon = go; }

void Start()
{
	pPhysic = m_pOwner->GetComponent<PhysicComponent>();
}

void Update()
{
	SetCurrentWeapon(GameManager::GetSceneManager().GetInventoryManager()->GetCurrentEquipedObject());

	HandleInput();

	if (GetKeyDown(KeyBinds::GetKeyBind(KeyAction::LockUnlockMouse)))
	{
		stopLookAround = !stopLookAround;
	}

	LookAround();
}

void Destroy()
{
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
	if (isJumping == false)
		jumpsAmount = 0;
}

END_SCRIPT
