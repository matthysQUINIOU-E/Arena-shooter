#include "UIManager.h"
#include "EntityWrapper.h"
#include "SceneManager.h"

#include "Scripts/UIGameplayBehavior.hpp"
#include "Scripts/UIMenuBehavior.hpp"

void UIManager::Init()
{
	EntityWrapper& owner = EntityWrapper::Create();
	owner.AddScript<UIGameplayBehavior>();
	owner.AddScript<UIMenuBehavior>();
}

bool UIManager::IsSceneType(SceneType type)
{
	return GameManager::GetSceneManager().GetSceneType() == type;
}
