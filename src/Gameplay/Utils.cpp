#include <Engine.h>
#include <fstream>
#include <RenderContext.h>
#include "json.h"
#include "Utils.h"


void ImportBlenderScene(std::wstring jsonFile)
{
	const std::string pathTexture = "res/Texture/";
	
	gce::GameManager* pGameManager = gce::GameManager::s_pInstance;
	gce::Scene* pScene = pGameManager->m_scenes[0]; // TODO :: changer la maniere dont recuperer la scene avec le SceneManager

	std::ifstream f(WRES_PATH L"res/BlenderScene/" + jsonFile);
	nlohmann::json data;

	try {
		data = nlohmann::json::parse(f);
	}
	catch (const std::exception& e) {
		std::cerr << "Erreur JSON: " << e.what() << std::endl;
		return;
	}

    for (auto& obj : data["objects"])
    {

        std::string name = obj.value("name", "Unnamed");
        std::string type = obj.value("type", "NONE");
        bool hasPhysic = obj.value("hasPhysic", false);
        bool hasCollider = obj.value("hasCollider", false);
        std::string material = obj.value("material", "");

        gce::Vector3f32 position;
        if (obj.contains("position"))
        {
            position.x = obj["position"][0];
            position.y = obj["position"][1];
            position.z = obj["position"][2];
        }

        gce::Quaternion rotation;
        if (obj.contains("rotation"))
            rotation = gce::Quaternion{ obj["rotation"][0], obj["rotation"][1], obj["rotation"][2], obj["rotation"][3] };
        else
            rotation = gce::Quaternion{ 0.f, 0.f, 0.f, 0.f };

        gce::Vector3f32 scale;
        if (obj.contains("scale"))
            scale = gce::Vector3f32{ obj["scale"][0], obj["scale"][1], obj["scale"][2] };
        else
            scale = gce::Vector3f32{ 1.f, 1.f, 1.f };

        std::string baseColorTex = "";
        std::string metallicTex = "";
        std::string roughnessTex = "";
        std::string normalMapTex = "";
        std::string emissionTex = "";

        if (obj.contains("textures"))
        {
            auto& tex = obj["textures"];

            if (tex.contains("base_color") && !tex["base_color"].is_null())
                baseColorTex = tex["base_color"].get<std::string>();

            if (tex.contains("metallic") && !tex["metallic"].is_null())
                normalMapTex = tex["metallic"].get<std::string>();

            if (tex.contains("roughness") && !tex["roughness"].is_null())
                normalMapTex = tex["roughness"].get<std::string>();

            if (tex.contains("normal_map") && !tex["normal_map"].is_null())
                normalMapTex = tex["normal_map"].get<std::string>();
        }

        gce::GameObject& gameObject = gce::GameObject::Create(*pScene);
        gameObject.SetName("importedScene"); // maybe change later just taging everything with the same name
        gameObject.transform.SetWorldPosition(position);
        gameObject.transform.SetWorldRotation(rotation);
        gameObject.transform.SetWorldScale(scale);


        gce::D12PipelineObject* defaultPso = new gce::D12PipelineObject( // TODO :: Change this because no delete now 
            gce::SHADERS.VERTEX,
            gce::SHADERS.PIXEL_TEXTURE,
            gce::SHADERS.HULL,
            gce::SHADERS.DOMAIN_,
            gce::SHADERS.ROOT_SIGNATURE_TEXTURE
        );

        std::vector<float> vertices;
        std::vector<uint32_t> indices;
        std::vector<float> uvs;

        if (obj.contains("mesh") && !obj["mesh"].is_null())
        {
            auto& mesh = obj["mesh"];

            if (mesh.contains("vertices"))
                vertices = mesh["vertices"].get<std::vector<float>>();

            if (mesh.contains("indices"))
                indices = mesh["indices"].get<std::vector<uint32_t>>();

            if (mesh.contains("uvs"))
                uvs = mesh["uvs"].get<std::vector<float>>();
        }

        gce::MeshRenderer* pMeshRenderer = gameObject.AddComponent<gce::MeshRenderer>();
        pMeshRenderer->pGeometry = MakeCustomGeometry(vertices, indices, uvs);
        pMeshRenderer->pPso = defaultPso;
        

        if (!baseColorTex.empty())
        {
            gce::Texture* pNewTexture = new gce::Texture(pathTexture + baseColorTex);
            pMeshRenderer->pMaterial->albedoTextureID = pNewTexture->GetTextureID();
            pMeshRenderer->pMaterial->useTextureAlbedo = 1;
        }

        if (!metallicTex.empty())
        {
            gce::Texture* pNewTexture = new gce::Texture(pathTexture + metallicTex);
            pMeshRenderer->pMaterial->metalnessTextureID = pNewTexture->GetTextureID();
            pMeshRenderer->pMaterial->useTextureMetalness = 1;
        }

        if (!roughnessTex.empty())
        {
            gce::Texture* pNewTexture = new gce::Texture(pathTexture + roughnessTex);
            pMeshRenderer->pMaterial->roughnessTextureID = pNewTexture->GetTextureID();
            pMeshRenderer->pMaterial->useTextureRoughness = 1;
        }

        if (!normalMapTex.empty())
        {
            gce::Texture* pNewTexture = new gce::Texture(pathTexture + normalMapTex);
            pMeshRenderer->pMaterial->normalTextureID = pNewTexture->GetTextureID();
            pMeshRenderer->pMaterial->useTextureNormal = 1;
        }

        if (hasCollider)
            gce::BoxCollider* pBoxCollider = gameObject.AddComponent<gce::BoxCollider>();

        if (hasPhysic)
            gce::PhysicComponent* pPhysic = gameObject.AddComponent<gce::PhysicComponent>();

    }
}

gce::Geometry* MakeCustomGeometry(std::vector<float> vertices, std::vector<uint32_t> indices, std::vector<float> uvs)
{
    gce::Geometry* customGeo = nullptr;
    gce::Vector<gce::Vertex> gceVertices;
    gce::Vector<uint32> gceIndices;

    size_t vertexCount = vertices.size() / 3;
    size_t uvCount = uvs.size() / 2;

    gceVertices.Resize(vertexCount);

    for (size_t i = 0; i < vertexCount; i++)
    {
        gce::Vertex v;
        v.pos = {
            vertices[i * 3 + 0],
            vertices[i * 3 + 1],
            vertices[i * 3 + 2]
        };

        v.normal = { 0.f, 0.f, 0.f }; // i don't know if it's right... but i have hopes :)

        v.uv = {
            uvs[i * 2 + 0],
            1.f - uvs[i * 2 + 1] 
        };

        gceVertices[i] = v;
    }

    gceIndices.Resize(indices.size());

    for (size_t i = 0; i < indices.size(); ++i)
        gceIndices[i] = indices[i];

    customGeo = new gce::Geometry(gceVertices.Data(), gceVertices.Size(), gceIndices.Data(), gceIndices.Size());
    return customGeo;
}