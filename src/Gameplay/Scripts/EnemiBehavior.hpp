#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include <numbers>


using namespace gce;

DECLARE_SCRIPT(EnemiBehavior, ScriptFlag::Start | ScriptFlag::Update)

GameObject* m_Target = nullptr;;
void Start()
{
	m_Target = GameManager::GetSceneManager().GetPlayer();
//	m_Target->transform.WorldRotate({ 0,45.f,0 });

}
void Moove(Vector3f32 Dir)
{
	float dt = GameManager::DeltaTime();
	float speed = 0.5f;

	m_pOwner->transform.LocalTranslate((Dir * speed * dt));


}
void Rotate()
{
	float pi = std::numbers::pi_v<float>;
	gce::Vector3f32 direction = {};
	direction = m_pOwner->transform.GetLocalPosition() - m_Target->transform.GetLocalPosition();
	float targetY = std::atan2(direction.x, direction.z);
	float currentY = m_pOwner->transform.GetLocalRotation().GetY();
	float TargetRotate = targetY - currentY;
	while (TargetRotate > pi)  TargetRotate -= 2 * pi;
	while (TargetRotate < -pi) TargetRotate += 2 * pi;
	float dt = GameManager::DeltaTime();
	float maxStep = 4.f * dt;
	m_pOwner->transform.LocalRotate({ 0,TargetRotate * maxStep,0 });
}
void Update()
{
	gce::Vector3f32 dir = {};
	gce::Vector3f32 finalDir = {};
	dir = m_pOwner->transform.GetLocalPosition() - m_Target->transform.GetLocalPosition();
	if (dir.x > 0.1f)
		finalDir.x = -1.0f;
	else if (dir.x < 0.1f)
		finalDir.x = 1.0f;
	//if (dir.y > 0)
	//	finalDir.y = -1.0f;
	//else if (dir.y < 0.5f)
	//	finalDir.y = 1.0f;
	if (dir.z > 0.1f)
		finalDir.z = -1.0f;
	else if (dir.z < 0.1f)
		finalDir.z = 1.0f;
	//Moove(finalDir);

	Rotate();
}

END_SCRIPT