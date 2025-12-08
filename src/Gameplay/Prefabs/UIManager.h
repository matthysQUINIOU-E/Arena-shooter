#pragma once
#include <iostream>
#include "EntityWrapper.h"

class UIManager
{
	EntityWrapper* m_pAmmoObj;
public:
	std::wstring ammoTxt;

	void CreateTexts();

	void UpdateUI();
};

