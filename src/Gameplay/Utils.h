#pragma once
#include <vector>

namespace gce
{
	class Geometry;
	class GameObject;
}

std::vector<gce::GameObject*> ImportBlenderScene(std::wstring jsonFile);
gce::Geometry* MakeCustomGeometry(std::vector<std::vector<float>> vertices, std::vector<uint32_t> indices);