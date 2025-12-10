#include "WaveManager.h"
#include "Scripts/AgentBehavior.hpp"
#include <time.h>

WaveManager* WaveManager::s_instance = nullptr;

WaveManager* WaveManager::GetInstance()
{
	if (s_instance == nullptr)
		s_instance = new WaveManager();

	return s_instance;
}

void WaveManager::Destroy()
{
	delete s_instance;
}

void WaveManager::Update()
{
	TryNextWave();
	TrySpawn();
}

void WaveManager::EnnemyKilled(Agent* ennemy)
{
	if (ennemy == nullptr)
		return;

	auto it = m_ennemiesFreePool.find(ennemy->GetUniqueTag({Tag::TMogwai, Tag::TGuHuoNiao, Tag::TJiangshi}));
	if (it != m_ennemiesFreePool.end())
	{
		it->second.push_back(ennemy);
		m_currentEnnemiesAlives--;
	}
}

WaveManager::WaveManager()
{
	std::random_device rand = std::random_device();
	m_rng = std::mt19937(rand());
	m_ennemyTag = { Tag::TMogwai, Tag::TJiangshi, Tag::TGuHuoNiao };
	m_ennemyTagDistrubution = std::uniform_int_distribution<int>(0, m_ennemyTag.size() - 1);

	gce::GameObject* player = gce::GameManager::GetSceneManager().GetFirstGameObject({ Tag::TPlayer });

	for (size_t i = 0; i < m_ennemyTag.size(); i++)
	{
		Tag ennemyTag = m_ennemyTag[i];
		for (size_t i = 0; i < m_maxSimultanateEnnemies; i++)
		{
			CreateEnnemy(ennemyTag, player);
		}
	}

	std::vector<GameObject*> spawners = gce::GameManager::GetSceneManager().GetAllGameObjects({ Tag::TSpawner });
	for (size_t i = 0; i < spawners.size(); i++)
	{
		gce::GameObject* spawner = spawners[i];
		for (size_t j = 0; j < m_ennemyTag.size(); j++)
		{
			Tag ennemyTag = m_ennemyTag[j];
			if (spawner->HasTags({ ennemyTag }))
				m_spawnerPosition[ennemyTag].push_back(spawner->transform.GetWorldPosition());
		}
	}

	for (size_t i = 0; i < m_ennemyTag.size(); i++)
	{
		Tag ennemyTag = m_ennemyTag[i];
		if (m_spawnerPosition[ennemyTag].empty())
			continue;
		m_spawnerPositionDistribution[ennemyTag] = std::uniform_int_distribution<int>(0, m_spawnerPosition[ennemyTag].size() - 1);
	}
}

void WaveManager::TryNextWave()
{
	if (m_curentWave == m_maxWave)
		return;

	if (m_currentEnnemiesAlives == 0 && m_currentEnnemiesToSpawn == 0)
		m_waveTimer = m_baseWaveTimer;

	if (m_waveTimer < m_baseWaveTimer)
	{
		m_waveTimer += gce::GameManager::DeltaTime();
		return;
	}

	m_waveTimer = 0.f;
	m_curentWave++;
	m_currentEnnemiesToSpawn += GetEnnemiesNumberForWave(m_curentWave);
	m_baseSpawnTimer = m_currentEnnemiesToSpawn / (m_baseWaveTimer * 0.25);
}

void WaveManager::TrySpawn()
{
	if (m_currentEnnemiesToSpawn == 0 || m_currentEnnemiesAlives >= m_maxSimultanateEnnemies)
		return; 

	if (m_spawnTimer < m_baseSpawnTimer)
	{
		m_spawnTimer += gce::GameManager::DeltaTime();
		return;
	}

	m_spawnTimer = 0.f;
	
	int tagIndex = m_ennemyTagDistrubution(m_rng);
	Tag tag = m_ennemyTag[tagIndex];
	Agent* ennemy = m_ennemiesFreePool[tag].back();
	m_ennemiesFreePool[tag].pop_back();
	ennemy->SetActive(true);

	int spawnPosIndex = m_spawnerPositionDistribution[tag](m_rng);
	gce::Vector3f32 spawnPos = m_spawnerPosition[tag][spawnPosIndex];
	ennemy->transform.SetWorldPosition(spawnPos);
	ennemy->SetCurrentNode(NavMesh::Instance()->GetNearestNodeFromPosition(spawnPos));
	m_currentEnnemiesAlives++;
}

int WaveManager::GetEnnemiesNumberForWave(int wave)
{
	return 8 + wave * 4;
}

void WaveManager::CreateEnnemy(Tag tag, gce::GameObject* player) //TODO :: create real ennemies
{
	Agent& entity = Agent::Create();
	AgentBehavior* ab = entity.AddScript<AgentBehavior>();
	entity.SetTarget(player);
	GameManager::GetSceneManager().LinkObjectToScene(&entity, SceneType::GamePlayScene);
	entity.SetActive(false);

	switch (tag)
	{
	case Tag::TMogwai:
		entity.AddMeshRenderer(gce::SHAPES.CUBE, "");
		entity.AddComponent<gce::BoxCollider>();
		break;
	case Tag::TJiangshi:
		entity.AddMeshRenderer(gce::SHAPES.CYLINDER, "");
		entity.AddComponent<gce::BoxCollider>();
		break;
	case Tag::TGuHuoNiao:
		entity.AddMeshRenderer(gce::SHAPES.SPHERE, "");
		entity.AddComponent<gce::BoxCollider>();
		break;
	default:
		break;
	}

	m_ennemiesFreePool[tag].push_back(&entity);
}
