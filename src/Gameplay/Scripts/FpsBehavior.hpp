#pragma once
#include <Render.h>
#include <Engine.h>
#include <Script.h>
#include <algorithm>
#include <string>
#include <iostream>

using namespace gce;

DECLARE_SCRIPT(FpsBehavior, ScriptFlag::Start | ScriptFlag::Update)

TextRenderer* txtRend = nullptr;

float mRefreshProgress = 0.f;

std::wstring* txtPointer = nullptr;

void Start()
{
	txtRend = m_pOwner->GetComponent<TextRenderer>();

	txtPointer = new std::wstring(L"");
}

void Update()
{
	if (mRefreshProgress < 0)
	{
		mRefreshProgress = 0.5f;

		if (txtRend)
		{
			*txtPointer = L"FPS : " + std::to_wstring((int)GameManager::FPS());
			txtRend->text = *txtPointer;
		}
	}
	else
	{
		mRefreshProgress -= GameManager::DeltaTime();
	}
}

END_SCRIPT
