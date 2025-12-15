#include "UIManager.h"
#include "EntityWrapper.h"
#include "SceneManager.h"

#include "Scripts/UIGameplayBehavior.hpp"
#include "Scripts/UIMenuBehavior.hpp"
#include "Scripts/UIGameOverBehavior.hpp"

void UIManager::Init()
{
	EntityWrapper& owner = EntityWrapper::Create();
	owner.AddScript<UIGameplayBehavior>();
	owner.AddScript<UIMenuBehavior>();
	owner.AddScript<UIGameOverBehavior>();
}

bool UIManager::IsSceneType(SceneType type)
{
	return GameManager::GetSceneManager().GetSceneType() == type;
}
