#pragma once
#include <vector>

namespace gce
{
	class Geometry;
	class GameObject;
}

std::vector<gce::GameObject*> ImportBlenderScene(std::wstring jsonFile);
gce::Geometry* MakeCustomGeometry(
    std::vector<float> vertices,
    std::vector<uint32_t> indices,
    gce::Vector<gce::Vertex>& gceVertices,
    gce::Vector<uint32>& gceIndices
);

gce::Vector3f32 ClosestPointOnLine(const gce::Vector3f32& A, const gce::Vector3f32& B, const gce::Vector3f32& P);
bool IsPointNearLine(const gce::Vector3f32& A, const gce::Vector3f32& B, const gce::Vector3f32& P, float radius);

