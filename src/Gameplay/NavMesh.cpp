#include "NavMesh.h"

#include <Components.h>
#include "GameManager.h"
#include "Utils.h"

NavMesh* NavMesh::s_pInstance = nullptr;

void NavMesh::Create(gce::Vector<gce::Vertex> vertices, gce::Vector<uint32> indices, std::vector<gce::GameObject*> obstacles)
{
	if (s_pInstance != nullptr)
	{
		delete s_pInstance;
	}
	s_pInstance = new NavMesh(vertices, indices, obstacles);
}

NavMesh* NavMesh::Instance()
{
	return s_pInstance;
}

void NavMesh::ResetVisited()
{
	if (++m_currentVersion == 0)
	{
		for (size_t i = 0; i < m_nodes.size(); i++)
		{
			Node<NavTile, Agent>* node = m_nodes[i];
			node->visitedVersion = 1;
		}
	}
}

Node<NavTile, Agent>* NavMesh::GetNearestNodeFromPosition(gce::Vector3f32 position)
{
	float bestDistance = std::numeric_limits<float>::infinity();
	NavTile* nearest;
	for (size_t i = 0; i < m_tiles.size(); i++)
	{
		NavTile* tile = m_tiles[i];
		float distance = (tile->GetPosition() - position).Norm();
		if (distance < bestDistance)
		{
			nearest = tile;
			bestDistance = distance;
		}
	}

	return m_mapTileNode[nearest];
}

bool NavMesh::DoesSegmentGoThroughObstacles(const gce::Vector3f32& A, const gce::Vector3f32& B, const float& radius)
{
	for (size_t i = 0; i < m_obstacles.size(); i++)
	{
		gce::GameObject* obstacle = m_obstacles[i];
		gce::Vector3f32 obstaclePos = obstacle->transform.GetWorldPosition();
		gce::MeshRenderer* mr = obstacle->GetComponent<gce::MeshRenderer>();
		gce::Geometry* geoObstacle = mr->pGeometry;
		const float32& maxX = geoObstacle->max.x + obstaclePos.x;
		const float32& maxZ = geoObstacle->max.z + obstaclePos.z;
		const float32& minX = geoObstacle->min.x + obstaclePos.x;
		const float32& minZ = geoObstacle->min.z + obstaclePos.z;

		if (IsPointNearLine(A, B, { maxX, 0.f, maxZ }, radius) ||
			IsPointNearLine(A, B, { minX, 0.f, maxZ }, radius) || 
			IsPointNearLine(A, B, { maxX, 0.f, minZ }, radius) || 
			IsPointNearLine(A, B, { minX, 0.f, minZ }, radius))
			
			return true;
	}
	return false;
}

uint32 NavMesh::GetCurrentVisitedVersion()
{
	return m_currentVersion;
}

NavMesh::NavMesh(gce::Vector<gce::Vertex> vertices, gce::Vector<uint32> indices, std::vector<gce::GameObject*> obstacles)
{
	m_obstacles = obstacles;
	size_t triangleNumber = indices.Size() / 3;
	std::unordered_map<std::string, std::vector<Node<NavTile, Agent>*>> mapIndicesNodes;

	for (size_t i = 0; i < triangleNumber; i++)
	{
		size_t indice1 = indices[i * 3];
		size_t indice2 = indices[i * 3 + 1];
		size_t indice3 = indices[i * 3 + 2];

		NavTile* navTile = new NavTile(vertices[indice1], vertices[indice2], vertices[indice3]);
		m_tiles.push_back(navTile);
		navTile->CheckObstacles(obstacles);

		Node<NavTile, Agent>* node = new Node<NavTile, Agent>{ navTile };
		m_nodes.push_back(node);

		m_mapTileNode[navTile] = node;

		std::string edge1 = std::to_string(min(indice1, indice2)) + std::to_string(max(indice1, indice2));
		std::string edge2 = std::to_string(min(indice2, indice3)) + std::to_string(max(indice2, indice3));
		std::string edge3 = std::to_string(min(indice1, indice3)) + std::to_string(max(indice1, indice3));

		auto it1 = mapIndicesNodes.find(edge1);
		if (it1 != mapIndicesNodes.end())
			it1->second.push_back(node);
		else
			mapIndicesNodes[edge1] = { node };

		auto it2 = mapIndicesNodes.find(edge2);
		if (it2 != mapIndicesNodes.end())
			it2->second.push_back(node);
		else
			mapIndicesNodes[edge2] = { node };

		auto it3 = mapIndicesNodes.find(edge3);
		if (it3 != mapIndicesNodes.end())
			it3->second.push_back(node);
		else
			mapIndicesNodes[edge3] = { node };
	}
	
	for (auto& [edge, nodes] : mapIndicesNodes)
	{
		if (nodes.size() == 2)
		{
			Node<NavTile, Agent>* a = nodes[0];
			Node<NavTile, Agent>* b = nodes[1];

			if (a->IsWalkable() && b->IsWalkable())
			{
				a->neighbors.insert(b);
				b->neighbors.insert(a);
			}
		}
	}
}

NavMesh::~NavMesh()
{
	for (auto& pair : m_mapTileNode)
	{
		delete pair.first;
		delete pair.second;
	}
}
