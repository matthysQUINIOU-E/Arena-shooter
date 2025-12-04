#pragma once
namespace gce
{
	class Geometry;
}

void ImportBlenderScene(std::wstring jsonFile);
gce::Geometry* MakeCustomGeometry(std::vector<float> vertices, std::vector<uint32_t> indices, std::vector<float> uvs);