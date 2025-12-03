#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include "Prefabs/ArenaCamera.h"
#include "Components.h"

using namespace gce;

DECLARE_SCRIPT(PlayerBehavior, ScriptFlag::Start | ScriptFlag::Update)

//Members
ArenaCamera* pCam = nullptr;

PhysicComponent* pPhysic = nullptr;


//Functions
inline void SetCam(ArenaCamera* pCamera)
{
	pCam = pCamera;

	pPhysic = m_pOwner->GetComponent<PhysicComponent>();
	pPhysic->SetBounciness(0.f);
}

inline void HandleInput()
{
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
		if (pPhysic)
		{
			gce::Force f;

			f.direction = { 0, 1, 0 };
			f.norm = 15000;
			f.useApplicationPoint = true; 
			f.relativeApplicationPoint = { 0, 0, 0 };

			pPhysic->AddForce(f);
		}
	}

	dir.SelfNormalize();

	m_pOwner->transform.WorldTranslate((dir * speed * dt));
}


inline void Start()
{
	m_pOwner->transform.SetWorldPosition({ 0, 0, 0 });

	if (pPhysic)
	{
		
	}
}

inline void Update()
{
	if (pCam != nullptr)
	{
		gce::Vector3f32 newPos = m_pOwner->transform.GetWorldPosition();
		newPos.z -= 10.f;

		pCam->GetGameObject()->transform.SetWorldPosition(newPos);
	}


	HandleInput();
}

END_SCRIPT
