#pragma once
#include <vector>
#include <Structs.h>
#include <NavTile.h>
#include <Node.h>
#include <Containers/Vector.hpp>

class Agent;

namespace gce
{
	class Geometry;
}

class NavMesh
{
public:
	static void Create(gce::Vector<gce::Vertex> vertices, gce::Vector<uint32> indices, std::vector<gce::Geometry*> obstacles);
	static NavMesh* Instance();
	void ResetVisited();
	Node<NavTile, Agent>* GetNearestNodeFromPosition(gce::Vector3f32 position); // clearly not optimal

private:
	NavMesh(gce::Vector<gce::Vertex> vertices, gce::Vector<uint32> indices, std::vector<gce::Geometry*> obstacles);
	~NavMesh();

private:
	static NavMesh* s_pInstance;
	std::vector<Node<NavTile, Agent>*> m_nodes;
	std::vector<NavTile*> m_tiles;
	std::unordered_map<NavTile*, Node<NavTile, Agent>*> m_mapTileNode;
};

