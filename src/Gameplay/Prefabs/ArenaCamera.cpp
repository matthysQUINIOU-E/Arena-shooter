#include "ArenaCamera.h"
#include "Scripts/CameraAnimationBehavior.hpp"
#include "EntityWrapper.h"

gce::GameObject* ArenaCamera::Create()
{
	m_pGameObject = &EntityWrapper::Create();

	m_pCamera = m_pGameObject->AddComponent<gce::Camera>();

	m_pCamera->SetMainCamera();
	m_pCamera->SetType(gce::PERSPECTIVE);
	m_pGameObject->AddScript<CameraAnimationBehavior>();

	return m_pGameObject;
}

void ArenaCamera::Shake()
{
	m_pGameObject->GetScript<CameraAnimationBehavior>()->TriggerShakingAnimation();
}

void ArenaCamera::SetParams(float32 fov, float32 nearplane, float32 farplane, float32 aspectRatio)
{
	if (m_pCamera == nullptr || m_pGameObject == nullptr)
		return;

	m_pCamera->perspective.fov = fov;
	m_pCamera->perspective.nearPlane = nearplane;
	m_pCamera->perspective.farPlane = farplane;
	m_pCamera->perspective.aspectRatio = aspectRatio;
	m_pCamera->perspective.up = { 0.0f, 1.0f, 0.0f };
}