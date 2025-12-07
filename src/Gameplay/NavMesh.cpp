#include "NavMesh.h"

NavMesh* NavMesh::s_pInstance = nullptr;

void NavMesh::Create(gce::Vector<gce::Vertex> vertices, gce::Vector<uint32> indices, std::vector<gce::Geometry*> obstacles)
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
	for (size_t i = 0; i < m_nodes.size(); i++)
	{
		Node<NavTile, Agent>* node = m_nodes[i];
		node->visited = false;
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

NavMesh::NavMesh(gce::Vector<gce::Vertex> vertices, gce::Vector<uint32> indices, std::vector<gce::Geometry*> obstacles)
{
	size_t triangleNumber = indices.Size() / 3;
	std::unordered_map<std::string, std::unordered_set<Node<NavTile, Agent>*>> mapIndicesNodes;

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
			it1->second.insert(node);
		else
			mapIndicesNodes[edge1] = { node };

		auto it2 = mapIndicesNodes.find(edge2);
		if (it2 != mapIndicesNodes.end())
			it2->second.insert(node);
		else
			mapIndicesNodes[edge2] = { node };

		auto it3 = mapIndicesNodes.find(edge3);
		if (it3 != mapIndicesNodes.end())
			it3->second.insert(node);
		else
			mapIndicesNodes[edge3] = { node };
	}
	
	for (auto& [edge, nodes] : mapIndicesNodes)
	{
		if (nodes.size() == 2)
		{
			auto it = nodes.begin();
			Node<NavTile, Agent>* a = *it;
			Node<NavTile, Agent>* b = *std::next(nodes.begin());

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
