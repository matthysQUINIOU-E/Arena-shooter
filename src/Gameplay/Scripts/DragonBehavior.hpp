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

float m_DetectionDistance = 30.f;
bool m_IsRotating = true;
int m_MaxFireBall;
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
gce::GameObject* m_Lazer;

StateMachinee m_stateMachine;
ReloadAmmo m_reloadAmmo;
void DragonRotateTowardDirection()
{
	GameObject* player = GameManager::GetSceneManager().GetFirstGameObject({ Tag::TPlayer });

	gce::Vector3f32 currentForward = m_pOwner->transform.GetWorldForward();
	currentForward.y = 0.f;
	float rotationSpeed = 3.f;
	if (currentForward.Norm() < 0.0001f)
		return;

	currentForward.SelfNormalize();

	gce::Vector3f32 targetForward =player->transform.GetWorldForward() ;
	targetForward.y = 0.f;
	targetForward.SelfNormalize();

	float dot = gce::Clamp(currentForward.DotProduct(targetForward), -1.f, 1.f);
	float angle = acosf(dot);

	gce::Vector3f32 cross = currentForward.CrossProduct(targetForward);
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
	GameObject * player = GameManager::GetSceneManager().GetFirstGameObject({ Tag::TPlayer });
	dir = m_pOwner->transform.GetWorldPosition()  - player->transform.GetWorldPosition();
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
	float speed = 0.5f;

	m_pOwner->transform.LocalTranslate((finalDir * speed * dt));
}
void CalculateNormal()
{
	gce::Vector3f32 dir = {};
	GameObject* player = GameManager::GetSceneManager().GetFirstGameObject({ Tag::TPlayer });
	dir = m_pOwner->transform.GetWorldPosition() - player->transform.GetWorldPosition();
	float distance = dir.Norm();
	if (distance <= m_DetectionDistance)
		m_EnnemiOnRange = true;
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
	ew.AddMeshRenderer(gce::GeometryFactory::GetCustomGeometry("res/Assets/Harpy/Attack/harpie_attack.obj"), "res/Assets/Harpy/Attack/harpie_attack_base_color.png"); // TODO :: projectile guhuoniao
	EnemyProjectileBehavior* epb = ew.AddScript<EnemyProjectileBehavior>();
	epb->m_baseLifeTime = m_projectileLifeTime;
	epb->m_speed = m_projectileSpeed;
	epb->m_damage = m_projectileSpeed;

	m_FireBalls.push_back(&ew);

}

void Update()
{
	CalculateNormal();
	if (!m_EnnemiOnRange)
		Move();
		DragonRotateTowardDirection();
}

void CollisionEnter()
{

}
END_SCRIPT