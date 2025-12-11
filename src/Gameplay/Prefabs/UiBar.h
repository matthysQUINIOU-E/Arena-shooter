#pragma once
#include "GameObject.h"
#include <iostream>
#include <utility>

class EntityWrapper;

struct TransformData
{
	gce::Vector2f32 pos;
	gce::Vector2f32 scale;
	float32 rotation;

	TransformData()
	{
		pos = {0, 0};
		scale = {0, 0};
		rotation = 0;
	}

	TransformData(gce::Vector2f32 _pos, gce::Vector2f32 _scale, float32 _rotation)
	{
		pos = _pos;
		scale = _scale;
		rotation = _rotation;
	}
};

class UiBar
{
	std::pair<EntityWrapper*, TransformData> m_emptyBar = { nullptr, TransformData() }; // The bar behind de dynamic bar, just for decoration too // Optional
	std::pair<EntityWrapper*, TransformData> m_filledBar = { nullptr, TransformData() }; // The dynamic Bar which is adapting to a ratio // The Core
	std::pair<EntityWrapper*, TransformData> m_frame = { nullptr, TransformData() };  // The decoration // Optional
public:
	void InitFrame(const char* imgPath, gce::Vector2f32 imgDimensions, gce::Vector2f32 pos = { 0, 0 }, gce::Vector2f32 scale = { 1, 1 }, gce::Vector2f32 rotation = { 0, 0 });
	void InitFilledBar(const char* imgPath, gce::Vector2f32 imgDimensions, gce::Vector2f32 pos = { 0, 0 }, gce::Vector2f32 scale = { 1, 1 }, gce::Vector2f32 rotation = { 0, 0 });
	void InitEmptyBar(const char* imgPath, gce::Vector2f32 imgDimensions, gce::Vector2f32 pos = { 0, 0 }, gce::Vector2f32 scale = { 1, 1 }, gce::Vector2f32 rotation = { 0, 0 });

	void SetFilledBarByRatio(float current, float max);
	void SetActive(bool state);
};

