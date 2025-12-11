#include "UiBar.h"
#include "EntityWrapper.h"

void UiBar::InitFrame(const char* imgPath, gce::Vector2f32 imgDimensions, gce::Vector2f32 pos, gce::Vector2f32 scale, gce::Vector2f32 rotation)
{
	auto& obj = m_frame.first;
	obj = &EntityWrapper::Create();

	obj->AddUIButton(pos, rotation, imgDimensions * scale, imgPath, imgPath);

	m_frame.second = TransformData(pos, imgDimensions * scale, rotation);
}

void UiBar::InitFilledBar(const char* imgPath, gce::Vector2f32 imgDimensions, gce::Vector2f32 pos, gce::Vector2f32 scale, gce::Vector2f32 rotation)
{
	auto& obj = m_filledBar.first;
	obj = &EntityWrapper::Create();

	obj->AddUIButton(pos, rotation, imgDimensions * scale, imgPath, imgPath);

	m_filledBar.second = TransformData(pos, imgDimensions * scale, rotation);
}

void UiBar::InitEmptyBar(const char* imgPath, gce::Vector2f32 imgDimensions, gce::Vector2f32 pos, gce::Vector2f32 scale, gce::Vector2f32 rotation)
{
	auto& obj = m_emptyBar.first;
	obj = &EntityWrapper::Create();
	
	obj->AddUIButton(pos, rotation, imgDimensions * scale, imgPath, imgPath);

	m_emptyBar.second = TransformData(pos, imgDimensions * scale, rotation);
}

void UiBar::SetFilledBarByRatio(float current, float max)
{
	auto obj = m_filledBar.first;
	auto data = m_filledBar.second;

	if (obj == nullptr)
		return;

	float ratio = current / max;
	float newScaleX = data.scale.x * ratio;
	float deltaScale = (data.scale.x - newScaleX) * 0.5f;

	obj->transform.SetWorldScale({ newScaleX, data.scale.y, 1 });
	obj->transform.SetWorldPosition({ data.pos.x - deltaScale, data.pos.y, 0});
}

void UiBar::SetActive(bool state)
{
	if (m_emptyBar.first)
		m_emptyBar.first->SetActive(state);

	if (m_filledBar.first)
		m_filledBar.first->SetActive(state);

	if (m_frame.first)
		m_frame.first->SetActive(state);
}