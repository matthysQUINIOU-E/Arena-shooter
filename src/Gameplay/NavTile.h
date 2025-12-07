#pragma once
#include <Structs.h>

namespace gce
{
	class Geometry;
}

class NavTile
{
public:
	NavTile(gce::Vertex& vertex1, gce::Vertex& vertex2, gce::Vertex& vertex3);
	void CheckObstacles(std::vector<gce::Geometry*> obstacles);
	const gce::Vector3f32& GetPosition() const;
	const float CalculateEuclidieanDistance(const NavTile& otherNavTile) const;
	const bool IsWalkable() const;
private:
	bool m_isWalkable = true;

	gce::Vector3f32 m_p1;
	gce::Vector3f32 m_p2;
	gce::Vector3f32 m_p3;

	gce::Vector3f32 m_center;

	gce::Vector3f32 m_max;
	gce::Vector3f32 m_min;
	// we don't care about Y because it's flat navmesh
};

