#pragma once
#include "Agent.h"

class Enemy
{
private:
	Enemy() = delete;

	static void CreateMogwai(Agent& agent);
	static void CreateJiangshi(Agent& agent);
	static void CreateGuHuoNiao(Agent& agent);
public:
	static Agent& CreateEnemy(gce::GameObject* target, Tag enemyType);
};

