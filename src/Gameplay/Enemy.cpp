#include "Enemy.h"
#include "Scripts/AgentBehavior.hpp"
#include "Scripts/GuhuoniaoBehavior.hpp"
#include "Scripts/JiangshiBehavior.hpp"
#include "Scripts/MogwaiBehavior.hpp"

void Enemy::CreateMogwai(Agent& agent)
{
    Geometry* customGeo = gce::GeometryFactory::GetCustomGeometry("res/Assets/mogwai_lowcost/mogwai_lowcost.obj");
    agent.AddMeshRenderer(customGeo, "res/Assets/mogwai_lowcost/mogwai_lowcost_base_color.png");
    agent.transform.WorldTranslate({ 0.f, abs(customGeo->max.y) + abs(customGeo->min.y), 0.f });
    agent.SetSpeed(5.f);
    agent.AddScript<MogwaiBehavior>();
}

void Enemy::CreateJiangshi(Agent& agent)
{
    Geometry* customGeo = gce::GeometryFactory::GetCustomGeometry("res/Assets/jiangshi/jiangshi.obj");
    agent.AddMeshRenderer(customGeo, "res/Assets/jiangshi/jiangshi_base_color.png");
    agent.transform.WorldTranslate({ 0.f, abs(customGeo->max.y) + abs(customGeo->min.y), 0.f });
    agent.SetSpeed(3.f);
    agent.AddScript<JiangshiBehavior>();
}

void Enemy::CreateGuHuoNiao(Agent& agent)
{

    Geometry* customGeo = gce::GeometryFactory::GetCustomGeometry("res/Assets/Harpy/harpy.obj");
    agent.AddMeshRenderer(customGeo, "res/Assets/Harpy/harpy_base_color.png");
    agent.transform.WorldTranslate({ 0.f, abs(customGeo->max.y) + abs(customGeo->min.y), 0.f });
    agent.SetSpeed(4.f);
    agent.AddScript<GuhuoniaBehavior>();
}

Agent& Enemy::CreateEnemy(gce::GameObject* target, Tag enemyType)
{
    Agent& agent = Agent::Create();
    agent.AddComponent<gce::BoxCollider>();
    agent.AddScript<AgentBehavior>();
    agent.SetTarget(target);

    switch (enemyType)
    {
    case Tag::TMogwai:
        CreateMogwai(agent);
        break;
    case Tag::TJiangshi:
        CreateJiangshi(agent);
        break;
    case Tag::TGuHuoNiao:
        CreateGuHuoNiao(agent);
        break;
    default:
        CreateMogwai(agent);
        break;
    }


    return agent;
}
