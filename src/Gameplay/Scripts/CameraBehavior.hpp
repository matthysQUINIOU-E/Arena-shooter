#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>

using namespace gce;

DECLARE_SCRIPT(CameraBehavior, ScriptFlag::Start | ScriptFlag::Update)

PrimaryTag priTag = PrimaryTag::None;
SecondaryTag secTag = SecondaryTag::None;

float sensitivity = 0.002f;

gce::Vector2i32 middleScreen = { (int)( (float)(WINDOW_WIDTH) * 0.5f), (int)((float)(WINDOW_HEIGHT) * 0.5f) };
bool fpsMode = true;

float totalPitchRotation = 0.f;

//Functions
void HandleFPSMode()
{
	if (m_pOwner->GetParent() == nullptr)
	{
		fpsMode = false;
		return;
	}

	gce::GameObject* pFollowGameObject = m_pOwner->GetParent();
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
	pFollowGameObject->transform.WorldRotate({ 0.f, yaw, 0.f });

	Quaternion pitchQ = Quaternion::RotationEuler({ totalPitchRotation, 0.f, 0.f});

	m_pOwner->transform.SetLocalRotation(pitchQ);

	SetCursorPos(middleScreen.x, middleScreen.y);
}

void HandleNormalMode()
{
	ShowMouseCursor();
}

void Start()
{
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
