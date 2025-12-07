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

void Start()
{
	txtRend = m_pOwner->GetComponent<TextRenderer>();
}

void Update()
{
	if (mRefreshProgress < 0)
	{
		mRefreshProgress = 0.5f;

		std::wstring tmp = std::to_wstring(10);

		if (txtRend)
		{
			txtRend->text = tmp;
		}
	}
	else
	{
		mRefreshProgress -= GameManager::DeltaTime();
	}
}

END_SCRIPT
