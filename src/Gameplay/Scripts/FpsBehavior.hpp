#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>

using namespace gce;

DECLARE_SCRIPT(FpsBehavior, ScriptFlag::Start | ScriptFlag::Update)

TextRenderer* txtRend = nullptr;

float mRefreshProgress = 0.f;

void Start()
{
	txtRend = m_pOwner->GetComponent<TextRenderer>();
}

void Update()
{
	if (mRefreshProgress < 0)
	{
		mRefreshProgress = 0.5f;

		std::wstring txt = L"FPS : " + std::to_wstring((int)GameManager::FPS());

		if (txtRend)
		{
			txtRend->text = txt;
		}
	}
	else
	{
		mRefreshProgress -= GameManager::DeltaTime();
	}
}

END_SCRIPT
