#pragma once
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include "Conditiones.h"
#include "Actiones.h"

enum class State
{
	MELEE_ATTACK,
	DISTANCE_ATTACK,
	MOVE
};

class StateMachinee
{
public:
	void Update();
	void SetState(State state); // force a state;
	void AddStateCondition(State beginState, State endState, Conditione& condition);
	void AddStateAction(State state, Actione& action);
private:
	std::unordered_map<State, std::vector<std::pair<Conditione*,State>>> m_stateConditionMap;
	std::unordered_map<State, std::vector<Actione*>> m_stateActionMap;
	State m_currentState;
};

