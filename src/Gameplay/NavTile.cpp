#include "NavTile.h"
#include <Geometry.h>

NavTile::NavTile(gce::Vertex& vertex1, gce::Vertex& vertex2, gce::Vertex& vertex3)
{
	m_p1 = vertex1.pos;
	m_p2 = vertex2.pos;
	m_p3 = vertex3.pos;

	m_center = (m_p1 + m_p2 + m_p3) / 3;

	m_max = {
		gce::Max(m_p1.x,gce::Max(m_p2.x,m_p3.x)),
		gce::Max(m_p1.y,gce::Max(m_p2.y,m_p3.y)),
		gce::Max(m_p1.z,gce::Max(m_p2.z,m_p3.z))
	};
	m_min = {
		gce::Min(m_p1.x,gce::Min(m_p2.x,m_p3.x)),
		gce::Min(m_p1.y,gce::Min(m_p2.y,m_p3.y)),
		gce::Min(m_p1.z,gce::Min(m_p2.z,m_p3.z))
	};
}

void NavTile::CheckObstacles(std::vector<gce::Geometry*> obstacles) // only check on x and z axis
{
	for (size_t i = 0; i < obstacles.size(); i++)
	{
		gce::Geometry* obstacle = obstacles[i];
		float32& maxX = obstacle->max.x;
		float32& maxZ = obstacle->max.z;
		float32& minX = obstacle->min.x;
		float32& minZ = obstacle->min.z;
		if (
			m_max.x <= maxX && m_max.x >= minX && m_max.z <= maxZ && m_max.z >= minZ ||
			m_min.x <= maxX && m_min.x >= minX && m_max.z <= maxZ && m_max.z >= minZ ||
			m_max.x <= maxX && m_max.x >= minX && m_min.z <= maxZ && m_min.z >= minZ ||
			m_min.x <= maxX && m_min.x >= minX && m_min.z <= maxZ && m_min.z >= minZ
		)
		{
			m_isWalkable = false;
			break;
		}
	}
}

const gce::Vector3f32& NavTile::GetPosition() const
{
	return m_center;
}

const float NavTile::CalculateEuclidieanDistance(const NavTile& otherNavTile) const
{
	float x = otherNavTile.GetPosition().x - m_center.x;
	float y = 0.f; // may be origine of not optinal path on plane navmesh otherNavTile.GetPosition().y - m_center.y;
	float z = otherNavTile.GetPosition().z - m_center.z;
	return std::sqrtf(x * x + y * y + z * z);
}

const bool NavTile::IsWalkable() const
{
	return m_isWalkable;
}
