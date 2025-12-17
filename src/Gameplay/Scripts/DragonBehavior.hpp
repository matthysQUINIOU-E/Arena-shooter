#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include "Prefabs/ArenaCamera.h"
#include "Components.h"
#include "EnemyScriptHelper.h"
#include "Scripts/EnemyProjectileBehavior.hpp"
using namespace gce;

DECLARE_SCRIPT(DragonBehavior, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

float m_Angle;
float m_DetectionRadius = 10.f;
bool m_IsRotating = true;
int m_MaxFireBall;
int m_Step = 0 ;
float m_rangeDistanceAttack;
int m_FireBallDamage;
int m_LaserDamage;
float m_projectileLifeTime;
float m_distanceAttackCooldown;
float m_projectileSpeed;
bool m_inAttack;
bool m_EnnemiOnRange = false ;

std::vector<gce::GameObject*> m_FireBalls;
std::unordered_set<gce::GameObject*> m_launchedFireBalls;
std::unordered_map<int, gce::Vector3f32> m_PointPosition;
std::unordered_map<int ,float > m_PointValue;
gce::GameObject* m_Player;
gce::GameObject* m_Lazer;
gce::Vector3f32 m_TargetForward;
StateMachinee m_stateMachine;
ReloadAmmo m_reloadAmmo;
void SetRotateToPlayer()
{
	m_TargetForward = m_pOwner->transform.GetWorldPosition();
	m_TargetForward.y = m_Player->transform.GetWorldPosition().y;
	m_TargetForward -= m_Player->transform.GetWorldPosition();
}
void SetRotateToTangeante()
{
	gce::Vector3f32 Distance = m_PointPosition[m_Step]- m_pOwner->transform.GetWorldPosition()  ;
	float normalize = Distance.Norm();
	if (normalize >= -2 && normalize <= 2)
		m_Step++;
	if (m_Step > 29)
	{
		m_Step = 0;
		m_Angle += m_PointValue[m_Step];
	}

	gce::Vector3f32 Rayon = m_PointPosition[m_Step] - m_Player->transform.GetWorldPosition();
	m_TargetForward.x = Rayon.z ;
	m_TargetForward.y = 0;
	m_TargetForward.z = Rayon.x * -1;
}
void CreatePointInCircle()
{
	int point = 30;
	for (int i = 0; i < point; ++i)
	{
		float theta = m_Angle + i * (2.0f * gce::PI / point);
		Vector3f32 point;
		point.x = m_Player->transform.GetWorldPosition().x + m_DetectionRadius * cos(theta);
		point.y = m_pOwner->transform.GetWorldPosition().y;
		point.z = m_Player->transform.GetWorldPosition().z + m_DetectionRadius * sin(theta);
		float angle = atan2(point.z - m_Player->transform.GetWorldPosition().z , point.x - m_Player->transform.GetWorldPosition().x);
		m_PointPosition[i] = point;

	}
	m_Step = 0;
}
void DragonRoamming()
{
	gce::Vector3f32 dir = {};
	dir = m_pOwner->transform.GetWorldPosition() - m_PointPosition[m_Step];
	gce::Vector3f32 finalDir = {};

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

	float dt = GameManager::DeltaTime();
	float speed = 2.f;

	m_pOwner->transform.WorldTranslate((finalDir * speed * dt));
}
void DragonRotateTowardDirection()
{

	gce::Vector3f32 currentForward = m_pOwner->transform.GetWorldForward();
	currentForward.y = 0.f;
	float rotationSpeed = 3.f;
	if (currentForward.Norm() < 0.0001f)
		return;

	currentForward.SelfNormalize();

	
	m_TargetForward.SelfNormalize();

	float dot = gce::Clamp(currentForward.DotProduct(m_TargetForward), -1.f, 1.f);
	float angle = acosf(dot);

	gce::Vector3f32 cross = currentForward.CrossProduct(m_TargetForward);
	float sign = (cross.y >= 0.f) ? 1.f : -1.f;

	float deltaYaw = angle * sign;

	float maxStep = rotationSpeed * gce::GameManager::DeltaTime();
	float step = gce::Clamp(deltaYaw, -maxStep, maxStep);

	m_pOwner->transform.WorldRotate({ 0.f, step, 0.f });

	if (fabs(deltaYaw) < 0.001f)
		m_IsRotating = false;
	else 
		m_IsRotating = true;
}
void Move()
{
	gce::Vector3f32 dir = {};
	dir = m_pOwner->transform.GetWorldPosition()  - m_Player->transform.GetWorldPosition();
	gce::Vector3f32 finalDir = {};

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

	float dt = GameManager::DeltaTime();
	float speed = 2.f;

	m_pOwner->transform.WorldTranslate((finalDir * speed * dt));
}
void CalculateNormal()
{
	gce::Vector3f32 dir = m_pOwner->transform.GetWorldPosition();
	dir.y = m_Player->transform.GetWorldPosition().y;
	dir -= m_Player->transform.GetWorldPosition();
	float distance = dir.Norm();
	if (distance <= m_DetectionRadius)
	{
		if (m_EnnemiOnRange)
		{
			m_Angle = std::atan2f(dir.z, dir.x);
			CreatePointInCircle();
		}
		m_EnnemiOnRange = true;
		
	}
	else
		m_EnnemiOnRange = false;
}
void Start()
{
	m_MaxFireBall = 5;
	m_rangeDistanceAttack = 10.f;
	m_FireBallDamage = 15;
	m_projectileLifeTime = 3.f;
	m_distanceAttackCooldown = 3.5f;
	m_projectileSpeed = 45.f;
	m_inAttack = false;
	EntityWrapper& ew = EntityWrapper::Create();
	ew.AddMeshRenderer(gce::GeometryFactory::GetCustomGeometry("res/Assets/FireBall/fireball.obj"), "res/Assets/FireBall/fireball_base_color.png"); // TODO :: projectile guhuoniao
	EnemyProjectileBehavior* epb = ew.AddScript<EnemyProjectileBehavior>();
	epb->m_baseLifeTime = m_projectileLifeTime;
	epb->m_speed = m_projectileSpeed;
	epb->m_damage = m_projectileSpeed;
	m_Player = GameManager::GetSceneManager().GetFirstGameObject({ Tag::TPlayer }); 
	m_FireBalls.push_back(&ew);
	SetRotateToPlayer();
}

void Update()
{
	CalculateNormal();
	DragonRotateTowardDirection();
	if (m_EnnemiOnRange)
	{
		SetRotateToTangeante();
		DragonRoamming();
	}
	else
	{
		Move();
		SetRotateToPlayer();
	}
}

void CollisionEnter()
{

}
END_SCRIPT