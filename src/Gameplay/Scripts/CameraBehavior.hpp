#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>

using namespace gce;

DECLARE_SCRIPT(CameraBehavior, ScriptFlag::Start | ScriptFlag::Update)

//Members
GameObject* pFollowGameObject = nullptr;

float sensitivity = 0.0025f;

gce::Vector2i32 middleScreen = { (int)( (float)(WINDOW_WIDTH) * 0.5f), (int)((float)(WINDOW_HEIGHT) * 0.5f) };
bool fpsMode = true;

float totalPitchRotation = 0.f;

//Functions
void SetGameObjectToFollow(GameObject* go) { pFollowGameObject = go; }

void HandleFPSMode()
{
	if (pFollowGameObject == nullptr)
		return;

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

	// Set Rotation for Both GameObjects
	pFollowGameObject->transform.LocalRotate({ 0.f, yaw, 0.f });
	m_pOwner->transform.SetLocalRotation({totalPitchRotation, 0.f, 0.f});

	// Recalculate the view after the FollowGameObject Rotation changes
	Quaternion yawQ = pFollowGameObject->transform.GetWorldRotation();
	Quaternion pitchQ = Quaternion::RotationEuler({ totalPitchRotation, 0.f, 0.f });

	m_pOwner->transform.SetWorldRotation(pitchQ * yawQ);


	// Update local positions for Weapons
	if (m_pOwner->GetChildren().Empty() == false)
	{
		for (GameObject* go : m_pOwner->GetChildren())
		{
			if (go->GetTag() == TWeapon)
			{
				go->transform.SetLocalPosition({ 0.25, -0.1f, 0.5f });
			}
		}
	}

	// Set Camera Pos
	gce::Vector3f32 newPos = pFollowGameObject->transform.GetWorldPosition();

	float heightOffset = pFollowGameObject->transform.GetWorldScale().y * 0.5f;

	newPos.y += heightOffset;
	m_pOwner->transform.SetWorldPosition(newPos);

	SetCursorPos(middleScreen.x, middleScreen.y);
}

void HandleNormalMode()
{
	ShowMouseCursor();
}

void Start()
{
	m_pOwner->transform.SetLocalRotation({ 0.f, 0.f, 0.f });
}

void Update()
{
	if (GetKeyDown(Keyboard::NUMPAD0))
	{
		fpsMode = !fpsMode;
	}

	if (fpsMode)
		HandleFPSMode();
	else
		HandleNormalMode();
}

END_SCRIPT
