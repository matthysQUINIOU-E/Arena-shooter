#pragma once
#include "GameObject.h"
#include "define.h"
#include "GameTransform.h"
#include "Components.h"
#include "Maths/Vector3.h"

class EntityWrapper : public gce::GameObject
{
public:
    EntityWrapper();

    static EntityWrapper& Create();
    EntityWrapper& SetProperties(const char* name, Tag1 tag1, Tag2 tag2, gce::Vector3f32 pos = {0, 0, 0}, gce::Vector3f32 rotation = {0, 0, 0}, gce::Vector3f32 scale = {1, 1, 1});
    EntityWrapper& SetChildProperties(EntityWrapper& parent, const char* name, Tag1 tag1, Tag2 tag2, gce::Vector3f32 pos = { 0, 0, 0 }, gce::Vector3f32 rotation = { 0, 0, 0 }, gce::Vector3f32 scale = { 1, 1, 1 });
    gce::MeshRenderer* AddMeshRenderer(gce::Geometry* pGeo,
        const char* albedoPath = "", const char* roughnessPath = "",
        const char* metalnessPath = "", const char* normalPath = "");

    gce::MeshRenderer* AddMeshRenderer(gce::Geometry* pGeo,
        gce::Texture* pAlbedo = nullptr, gce::Texture* pRoughness = nullptr,
        gce::Texture* pMetalness = nullptr, gce::Texture* pNormal = nullptr);

    gce::PhysicComponent* AddPhysics(float32 mass, float32 gravityScale, float32 bounciness);

    gce::TextRenderer* AddTextRenderer(std::wstring txt, gce::RectanglePosF dimensions = {0, 0, 200, 200}, gce::Color txtColor = gce::Color::Black, std::wstring fontName = L"Arial");
};