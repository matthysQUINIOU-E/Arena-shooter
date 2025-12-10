#include "UiBar.h"
#include "EntityWrapper.h"

void UiBar::InitFrame(const char* imgPath, gce::Vector2f32 leftTopRectPos, gce::Vector2f32 imgDimensions, gce::Vector2f32 pos, gce::Vector2f32 scale, float32 rotation)
{
	auto& obj = m_frame.first;
	obj = &EntityWrapper::Create();
	obj->AddUiImage(imgPath, leftTopRectPos, imgDimensions, pos, scale, rotation);

	m_frame.second = TransformData(pos, scale, rotation);
}

void UiBar::InitFilledBar(const char* imgPath, gce::Vector2f32 leftTopRectPos, gce::Vector2f32 imgDimensions, gce::Vector2f32 pos, gce::Vector2f32 scale, float32 rotation)
{
	auto& obj = m_filledBar.first;
	obj = &EntityWrapper::Create();
	obj->AddUiImage(imgPath, leftTopRectPos, imgDimensions, pos, scale, rotation);

	m_filledBar.second = TransformData(pos, scale, rotation);
}

void UiBar::InitEmptyBar(const char* imgPath, gce::Vector2f32 leftTopRectPos, gce::Vector2f32 imgDimensions, gce::Vector2f32 pos, gce::Vector2f32 scale, float32 rotation)
{
	auto& obj = m_emptyBar.first;
	obj = &EntityWrapper::Create();
	obj->AddUiImage(imgPath, leftTopRectPos, imgDimensions, pos, scale, rotation);

	m_emptyBar.second = TransformData(pos, scale, rotation);
}

void UiBar::SetFilledBarByRatio(float current, float max)
{
	auto obj = m_filledBar.first;
	auto data = m_filledBar.second;

	if (obj == nullptr)
		return;

	float ratio = current / max;

	obj->SetUiImageTransform(data.pos, { data.scale.x * ratio, data.scale.y }, data.rotation);
}

void UiBar::SetActive(bool state)
{
	if (m_emptyBar.first)
		m_frame.first->SetActive(state);

	if (m_filledBar.first)
		m_frame.first->SetActive(state);

	if (m_frame.first)
		m_frame.first->SetActive(state);
}