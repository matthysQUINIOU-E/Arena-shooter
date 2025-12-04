#include "EntityWrapper.h"
#include "GameObject.h"
#include "Script.h"
#include "GameManager.h"

EntityWrapper::EntityWrapper() : gce::GameObject()
{
}

EntityWrapper& EntityWrapper::Create()
{
	EntityWrapper* const pNew = new EntityWrapper();
	pNew->m_pScene = &gce::GameManager::GetScene();
	pNew->transform.m_pOwner = pNew;
	gce::GameManager::GetLifespanSystem().m_toCreate.gameObjects.Push(pNew);
	return *pNew;
}

EntityWrapper& EntityWrapper::SetProperties(const char* name, Tag tag, gce::Vector3f32 pos, gce::Vector3f32 rotation, gce::Vector3f32 scale)
{
	SetName(name);
	SetTag(tag);
	transform.SetWorldPosition(pos);
	transform.SetWorldRotation(rotation);
	transform.SetWorldScale(scale);

	return *this;
}

EntityWrapper& EntityWrapper::SetChildProperties(EntityWrapper& parent, const char* name, Tag tag, gce::Vector3f32 pos, gce::Vector3f32 rotation, gce::Vector3f32 scale)
{
	parent.AddChild(*this);

	SetName(name);
	SetTag(tag);
	transform.SetLocalPosition(pos);
	transform.SetLocalRotation(rotation);
	transform.SetLocalScale(scale);

	return *this;
}

gce::MeshRenderer* EntityWrapper::AddMeshRenderer(gce::Geometry* pGeo, const char* albedoPath, const char* roughnessPath, const char* metalnessPath, const char* normalPath)
{
	gce::MeshRenderer* component = AddComponent<gce::MeshRenderer>();

	component->pGeometry = pGeo;

	if (albedoPath != "")
	{
		gce::Texture* pAlbedo = new gce::Texture(albedoPath);
		component->pMaterial->albedoTextureID = pAlbedo->GetTextureID();

	}
	else
	{
		component->pMaterial->albedoTextureID = gce::GameManager::s_pInstance->m_pWhiteTexture->GetTextureID();
	}

	component->pMaterial->useTextureAlbedo = 1;

	if (roughnessPath != "")
	{
		gce::Texture* pRoughness = new gce::Texture(roughnessPath);
		component->pMaterial->roughnessTextureID = pRoughness->GetTextureID();
		component->pMaterial->useTextureRoughness = 1;
	}

	if (metalnessPath != "")
	{
		gce::Texture* pMetalness = new gce::Texture(metalnessPath);
		component->pMaterial->metalnessTextureID = pMetalness->GetTextureID();
		component->pMaterial->useTextureMetalness = 1;
	}

	if (normalPath != "")
	{
		gce::Texture* pNormal = new gce::Texture(normalPath);
		component->pMaterial->normalTextureID = pNormal->GetTextureID();
		component->pMaterial->useTextureNormal = 1;
	}

	component->pMaterial->subsurface = 1;
	component->pPso = gce::GameManager::GetSceneManager().GetPSO();

	return component;
}

gce::MeshRenderer* EntityWrapper::AddMeshRenderer(gce::Geometry* pGeo, gce::Texture* pAlbedo, gce::Texture* pRoughness, gce::Texture* pMetalness, gce::Texture* pNormal)
{
	gce::MeshRenderer* component = AddComponent<gce::MeshRenderer>();

	component->pGeometry = pGeo;

	if (pAlbedo)
	{
		component->pMaterial->albedoTextureID = pAlbedo->GetTextureID();
	}
	else
	{
		component->pMaterial->albedoTextureID = gce::GameManager::s_pInstance->m_pWhiteTexture->GetTextureID();
	}

	component->pMaterial->useTextureAlbedo = 1;

	if (pRoughness)
	{
		component->pMaterial->roughnessTextureID = pRoughness->GetTextureID();
		component->pMaterial->useTextureRoughness = 1;
	}

	if (pMetalness)
	{
		component->pMaterial->metalnessTextureID = pMetalness->GetTextureID();
		component->pMaterial->useTextureMetalness = 1;
	}

	if (pNormal)
	{
		component->pMaterial->normalTextureID = pNormal->GetTextureID();
		component->pMaterial->useTextureNormal = 1;
	}

	component->pMaterial->subsurface = 1;
	component->pPso = gce::GameManager::GetSceneManager().GetPSO();

	return component;
}

gce::PhysicComponent* EntityWrapper::AddPhysics(float32 mass, float32 gravityScale, float32 bounciness)
{
	gce::PhysicComponent* component = AddComponent<gce::PhysicComponent>();

	component->SetMass(mass);
	component->SetGravityScale(gravityScale);
	component->SetBounciness(bounciness);

	return component;
}

gce::TextRenderer* EntityWrapper::AddTextRenderer(std::wstring txt, gce::RectanglePosF dimensions, gce::Color txtColor, std::wstring fontName)
{
	gce::TextRenderer* component = AddComponent<gce::TextRenderer>();

	component->pFont = new gce::Font(fontName);
	component->pBrush = new gce::ColorBrush(txtColor);
	component->text = txt;
	component->rectPosF = new gce::RectanglePosF(dimensions);

	return component;
}

///////////////////////////////////////////////////////////////////
