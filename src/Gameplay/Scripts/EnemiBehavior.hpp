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
	float pi = std::numbers::pi_v<float>;
	//m_Target = GameManager::GetSceneManager().GetPlayer();
	m_Target->transform.LocalRotate({ 0,-7,0});

}
void Moove(Vector3f32 Dir)
{
	float dt = GameManager::DeltaTime();
	float speed = 0.5f;

	m_pOwner->transform.LocalTranslate((Dir * speed * dt));


}
void Rotate()
{
	if (m_pOwner->transform.GetWorldRotation().GetY() == m_Target->transform.GetWorldRotation().GetY())
		return;
	float pi = std::numbers::pi_v<float>;
	gce::Vector3f32 direction = {};
	float currentY = -m_Target->transform.GetWorldRotation().GetY() * 1.f / 2*pi;
	std::cout << m_pOwner->transform.GetWorldRotation().GetY() << "   "<< m_Target->transform.GetWorldRotation().GetY()<< std::endl;
	if (currentY < 0)
	{
		currentY -= 2 * pi; 
	}
	m_pOwner->transform.SetWorldRotation({ 0,m_pOwner->transform.GetWorldRotation().GetY() +currentY,0 });
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

	//Rotate();
}

END_SCRIPT