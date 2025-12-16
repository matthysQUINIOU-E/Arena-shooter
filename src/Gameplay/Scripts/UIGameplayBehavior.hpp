#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include "Components.h"
#include "GameManager.h"
#include "SceneManager.h"

#include "Prefabs/EntityWrapper.h"

#include "Scripts/WeaponMagazineBehavior.hpp"
#include "Scripts/GunBehavior.hpp"
#include "Scripts/HealthBehavior.hpp"
#include "Scripts/PlayerBehavior.hpp"

#include "../Prefabs/InventoryManager.h"
#include "../Prefabs/UIManager.h"
#include "../Prefabs/UiBar.h"
#include "../Prefabs/CrosshairManager.h"
#include "../WaveManager.h"

using namespace gce;

DECLARE_SCRIPT(UIGameplayBehavior, ScriptFlag::Start | ScriptFlag::Update)

UIManager* pUIManager = nullptr;

//Members
EntityWrapper* pFpsUI = nullptr;
std::wstring fpsTxt;
float mRefreshProgress = 0.f;

EntityWrapper* pAmmoUI = nullptr;
std::wstring ammoTxt;

EntityWrapper* pTotalAmmoUI = nullptr;
std::wstring totalAmmoTxt;

CrosshairManager crosshair;

UiBar hpBar;
UiBar dashBar;

std::wstring NOTHING = L"";

EntityWrapper* pTakeDamageUI = nullptr;
int oldPlayerHp = -1;
bool takeDamageTriggerVisual = false;
float takeDamageDuration = 1.f;
float takeDamageProgressDuration;


//TMP
EntityWrapper* pDebugUI = nullptr;
std::wstring debugTxt = L"";

//Functions
void UpdateTakeDamageUI()
{
	auto pPlayer = GameManager::GetSceneManager().GetFirstGameObject({ Tag::TPlayer });

	if (takeDamageTriggerVisual)
	{
		if (takeDamageProgressDuration < takeDamageDuration)
		{
			takeDamageProgressDuration += GameManager::DeltaTime();
			pTakeDamageUI->SetActive(true);
		}
		else
		{
			takeDamageProgressDuration = 0.f;
			takeDamageTriggerVisual = false;
			pTakeDamageUI->SetActive(false);
		}
	}

	if (HealthBehavior* pScript = pPlayer->GetScript<HealthBehavior>())
	{
		if (oldPlayerHp == -1)
		{
			oldPlayerHp = pScript->health;
			return;
		}

		if (pScript->health < oldPlayerHp || pScript->health <= 0)
		{
			oldPlayerHp = pScript->health;
			takeDamageProgressDuration = 0.f;
			takeDamageTriggerVisual = true;
		}
	}
}

void UpdateFpsUI()
{
	if (mRefreshProgress < 0)
	{
		mRefreshProgress = 0.5f;

		fpsTxt = L"FPS : " + std::to_wstring((int)GameManager::FPS());
		pFpsUI->UpdateDynamicText(fpsTxt);
	}
	else
	{
		mRefreshProgress -= GameManager::DeltaTime();
	}
}
void UpdateAmmosUI()
{
	gce::GameObject* pObj = GameManager::GetSceneManager().GetInventoryManager()->GetCurrentEquipedObject();

	if (pObj == nullptr)
		return;

	if (pObj->HasTags({ Tag::TWeapon }))
	{
		WeaponMagazineBehavior* pScript = pObj->GetScript<WeaponMagazineBehavior>();

		if (pScript)
		{
			ammoTxt = L"Ammos : " + std::to_wstring(pScript->ammosLeft) + L"/" + std::to_wstring(pScript->maxCapacity);
		}
		else
		{
			ammoTxt = NOTHING;
		}
	}
	else
	{
		ammoTxt = L"To Do";
	}

	pAmmoUI->UpdateDynamicText(ammoTxt);
}
void UpdateTotalAmmoUI()
{
	InventoryManager* pInventory = GameManager::GetSceneManager().GetInventoryManager();

	gce::GameObject* pObj = pInventory->GetCurrentEquipedObject();

	if (pObj == nullptr)
		return;

	if (pObj->HasTags({ Tag::TWeapon }))
	{
		WeaponMagazineBehavior* pScript = pObj->GetScript<WeaponMagazineBehavior>();

		if (pScript)
		{
			Ammos* pAmmoToDisplay = pInventory->GetAmmos(pScript->typeOfAmmo);

			if (pAmmoToDisplay)
			{
				totalAmmoTxt = L"Stock : " + std::to_wstring(pAmmoToDisplay->GetAmount());
			}
			else
			{
				totalAmmoTxt = NOTHING;
			}
		}
		else
		{
			totalAmmoTxt = NOTHING;
		}
	}
	else
	{
		totalAmmoTxt = L"To Do";
	}

	pTotalAmmoUI->UpdateDynamicText(totalAmmoTxt);
}
void UpdateDashUI()
{
	gce::GameObject* pPlayer = GameManager::GetSceneManager().GetFirstGameObject({ Tag::TPlayer });

	if (pPlayer == nullptr)
		return;

	PlayerBehavior* script = pPlayer->GetScript<PlayerBehavior>();

	if (script)
	{
		int dashAmount = script->dashAmount;
		int dashReloadTimeHalf = script->dashTotalReloadTime / script->maxDashAmount;

		if (script->dashProgressReloadTime < dashReloadTimeHalf)
		{
			dashBar.SetFilledBar1ByRatio(script->dashProgressReloadTime, dashReloadTimeHalf);
			dashBar.SetFilledBar2ByRatio(0.f, 1.f);
		}
		else
		{
			dashBar.SetFilledBar1ByRatio(1.f, 1.f);
			dashBar.SetFilledBar2ByRatio(-dashReloadTimeHalf + script->dashProgressReloadTime, dashReloadTimeHalf);
		}
	}
}
void UpdateHpUI()
{
	gce::GameObject* pPlayer = GameManager::GetSceneManager().GetFirstGameObject({ Tag::TPlayer });

	if (pPlayer == nullptr)
		return;

	HealthBehavior* health = pPlayer->GetScript<HealthBehavior>();

	if (health)
	{
		hpBar.SetFilledBar1ByRatio(health->health, health->maxHealth);
	}
}
void UpdateCrosshair()
{
	crosshair.Update();
}

void Start()
{
	pUIManager = GameManager::GetSceneManager().GetUIManager();

	pFpsUI = &EntityWrapper::Create();
	pFpsUI->AddDynamicTextRenderer(fpsTxt, { 1350, 10, 300, 300 }, gce::Color::Black);

	pAmmoUI = &EntityWrapper::Create();
	gce::Vector3f32 ammoPos = { 1480, 900, 0.f };
	pAmmoUI->AddDynamicTextRenderer(ammoTxt, { ammoPos.x, ammoPos.y, 0, 0 }, gce::Color::Red);

	pTotalAmmoUI = &EntityWrapper::Create();
	gce::Vector3f32 totalAmmmoPos = { 1480, 850, 0.f };
	pTotalAmmoUI->AddDynamicTextRenderer(totalAmmoTxt, { totalAmmmoPos.x, totalAmmmoPos.y, 0, 0 }, gce::Color::Red);

	pTakeDamageUI = &EntityWrapper::Create();
	gce::Vector2f32 takeDamagePos = { WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f };
	pTakeDamageUI->AddUIButton(takeDamagePos, { 0, 0 }, { WINDOW_WIDTH, WINDOW_HEIGHT }, "res/2D_Assets/takedamage_screen.png");
	pTakeDamageUI->SetActive(false);

	pDebugUI = &EntityWrapper::Create();
	pDebugUI->AddDynamicTextRenderer(debugTxt, {0, 100, 500, 0}, gce::Color::Red);
	crosshair.Init();

	hpBar.InitFilledBar1("res/2D_Assets/hpBar.png", {405, 53}, { 190, 69}, { 0.5, 0.5 });
	hpBar.InitFrame("res/2D_Assets/hpBar_frame.png", { 569, 204 }, { 160, 60 }, { 0.5, 0.5 });

	dashBar.InitFilledBar1("res/2D_Assets/dashBar.png", { 416, 63 }, { 218, 170 }, { 0.4, 0.4 });
	dashBar.InitFilledBar2("res/2D_Assets/dashBar_full.png", { 416, 63 }, { 218, 170 }, { 0.4, 0.4 });
	dashBar.InitFrame("res/2D_Assets/dashBar_frame.png", { 704, 186 }, { 160, 160 }, { 0.4, 0.4 });
}

void Update()
{
	bool display = pUIManager->IsSceneType(SceneType::GamePlayScene);

	pAmmoUI->SetActive(display);
	pTotalAmmoUI->SetActive(display);
	pFpsUI->SetActive(display);
	crosshair.SetActive(display);
	hpBar.SetActive(display);
	dashBar.SetActive(display);

	if (display == true)
	{
		UpdateFpsUI();
		UpdateAmmosUI();
		UpdateTotalAmmoUI();
		UpdateCrosshair();
		UpdateHpUI();
		UpdateDashUI();
		UpdateTakeDamageUI();

		auto wave = WaveManager::GetInstance();

		debugTxt = L"Time : " + std::to_wstring((int)wave->m_waveTimer) + L"\n"
			+ L"Wave : " + std::to_wstring(wave->m_curentWave) + L"/" + std::to_wstring(wave->m_maxWave) + L"\n"
			+ L"Enemies To Kill : " + std::to_wstring(wave->m_currentEnnemiesToSpawn);

		pDebugUI->GetComponent<TextRenderer>()->text = debugTxt;
	}
	else
	{
		pTakeDamageUI->SetActive(false);
		takeDamageTriggerVisual = false;
	}
}

END_SCRIPT
