#include <fstream>
#include <regex>
#include <Engine.h>
#include <RenderContext.h>
#include "json.h"
#include "Utils.h"
#include "NavMesh.h"

std::vector<gce::GameObject*> ImportBlenderScene(std::wstring jsonFile)
{
	const std::string pathTexture = "res/Texture/";
	
	gce::GameManager* pGameManager = gce::GameManager::s_pInstance;
    gce::Scene& pScene = pGameManager->GetScene();

	std::ifstream f(WRES_PATH L"res/BlenderScene/" + jsonFile);
	nlohmann::json data;

	try {
		data = nlohmann::json::parse(f);
	}
	catch (const std::exception& e) {
		std::cerr << "Erreur JSON: " << e.what() << std::endl;
		return std::vector<gce::GameObject*>();
	}

    std::regex patternPhysic("^[^_]*P");
    std::regex patternCollider("^[^_]*C");
    std::regex patternDestructible("^[^_]*D");
    std::regex patternNavMesh("^[^_]*NM");
    std::regex patternSpawnerHeal("^[^_]*SH");
    std::regex patternSpawnerGuHuoNiao("^[^_]*SG");
    std::regex patternSpawnerJiangshi("^[^_]*SJ");
    std::regex patternSpawnerMogwai("^[^_]*SM");

    gce::Vector<gce::Vertex> navmeshVertices;
    gce::Vector<uint32> navmeshIndices;
    std::vector<gce::GameObject*> obstaclesGameObject;

    std::vector<gce::GameObject*> allCreatedObj;

    for (auto& obj : data)
    {
        std::string type = obj.value("t", "NONE");
        if (type != "MESH")
            continue;

        std::string name = obj.value("n", "Unnamed");
        bool hasPhysic = std::regex_search(name, patternPhysic);
        bool hasCollider = std::regex_search(name, patternCollider);
        bool isDestructible = std::regex_search(name, patternDestructible);
        bool isNavMesh = std::regex_search(name, patternNavMesh);
        bool isSpawnerHeal = std::regex_search(name, patternSpawnerHeal);
        bool isSpawnerGuHuoNiao = std::regex_search(name, patternSpawnerGuHuoNiao);
        bool isSpawnerJiangshi = std::regex_search(name, patternSpawnerJiangshi);
        bool isSpawnerMogwai = std::regex_search(name, patternSpawnerMogwai);
        bool isobstacle = !isNavMesh && hasCollider;
        std::string baseColorTex = "";
        std::string metallicTex = "";
        std::string roughnessTex = "";
        std::string normalMapTex = "";
        std::string emissionTex = "";
        gce::Vector3f32 position;
        gce::Quaternion rotation;
        gce::Vector3f32 scale;

        if (obj.contains("tx"))
        {
            auto& tex = obj["tx"];

            if (tex.contains("Base Color"))
                baseColorTex = tex["Base Color"].get<std::string>();

           /* if (tex.contains("metallic") && !tex["metallic"].is_null())
                metallicTex = tex["metallic"].get<std::string>();

            if (tex.contains("Roughness"))
                roughnessTex = tex["Roughness"].get<std::string>();

            if (tex.contains("normal_map") && !tex["normal_map"].is_null())
                normalMapTex = tex["normal_map"].get<std::string>();*/
        }

        if (obj.contains("p"))
        {
            position.x = obj["p"][0];
            position.y = obj["p"][2];
            position.z = obj["p"][1];
        }

        if (obj.contains("r"))
            rotation = gce::Quaternion{ obj["r"][0], obj["r"][2], obj["r"][1], obj["r"][3] };
        else
            rotation = gce::Quaternion{ 0.f, 0.f, 0.f, 0.f };

        if (obj.contains("s"))
            scale = gce::Vector3f32{ obj["s"][0], obj["s"][2], obj["s"][1] };
        else
            scale = gce::Vector3f32{ 1.f, 1.f, 1.f };

        gce::GameObject& gameObject = gce::GameObject::Create(pScene);

        if (isSpawnerHeal || isSpawnerGuHuoNiao || isSpawnerJiangshi || isSpawnerMogwai) {
            gameObject.AddTags({ Tag::TSpawner });
            if (isSpawnerHeal)
                gameObject.AddTags({ Tag::THeal });
            if (isSpawnerGuHuoNiao)
                gameObject.AddTags({ Tag::TGuHuoNiao });
            if (isSpawnerJiangshi)
                gameObject.AddTags({ Tag::TJiangshi });
            if (isSpawnerMogwai)
                gameObject.AddTags({Tag::TMogwai});
        }
        if (isDestructible)
            gameObject.AddTags({ Tag::TDestructible });

        // gameObject.SetName("importedScene"); // maybe change later just taging everything with the same name
        gameObject.transform.SetWorldPosition(position);
        gameObject.transform.SetWorldRotation(rotation);
        gameObject.transform.SetWorldScale(scale);

        std::vector<float> vertices;
        std::vector<uint32_t> indices;

        if (obj.contains("m"))
        {
            auto& mesh = obj["m"];

            if (mesh.contains("v"))
                vertices = mesh["v"].get<std::vector<float>>();

            if (mesh.contains("i"))
                indices = mesh["i"].get<std::vector<uint32_t>>();
        }

        gce::MeshRenderer* pMeshRenderer = gameObject.AddComponent<gce::MeshRenderer>();
        gce::Vector<gce::Vertex> gceVertices;
        gce::Vector<uint32> gceIndices;
        pMeshRenderer->pGeometry = MakeCustomGeometry(vertices, indices, gceVertices, gceIndices);
        pMeshRenderer->pPso = pGameManager->GetSceneManager().GetPSO();
        pMeshRenderer->pMaterial->subsurface = 1;

        if (isNavMesh)
        {
            navmeshVertices = gceVertices;
            navmeshIndices = gceIndices;
        }
        else if (hasCollider)
        {
            obstaclesGameObject.push_back(&gameObject);
        }
        
        if (!baseColorTex.empty())
        {
            gce::Texture* pNewTexture = new gce::Texture(pathTexture + baseColorTex);
            pMeshRenderer->pMaterial->albedoTextureID = pNewTexture->GetTextureID();
            pMeshRenderer->pMaterial->useTextureAlbedo = 1;
        }
        /*
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
        */
        if (hasCollider)
            gce::BoxCollider* pBoxCollider = gameObject.AddComponent<gce::BoxCollider>();

        if (hasPhysic)
            gce::PhysicComponent* pPhysic = gameObject.AddComponent<gce::PhysicComponent>();

        allCreatedObj.push_back(&gameObject);
    }

    if (!navmeshVertices.Empty())
    {
        NavMesh::Create(navmeshVertices, navmeshIndices, obstaclesGameObject);
    }
    
    return allCreatedObj;
}

gce::Geometry* MakeCustomGeometry(
    std::vector<float> vertices,
    std::vector<uint32_t> indices,
    gce::Vector<gce::Vertex>& gceVertices,
    gce::Vector<uint32>& gceIndices
)
{
    gce::Geometry* customGeo = nullptr;

    size_t vertexCount = vertices.size() / 5;

    gceVertices.Resize(vertexCount);

    for (size_t i = 0; i < vertexCount; i++)
    {
        size_t vIndex = i * 5;
        gce::Vertex v;
        v.pos = {
            vertices[vIndex],
            vertices[vIndex + 2],
            vertices[vIndex + 1]
        };

        v.normal = { 0.f, 0.f, 0.f }; // it's not right but it will stay like this for now

        v.uv = {
            vertices[vIndex + 3],
            1.f - vertices[vIndex + 4]
        };

        gceVertices[i] = v;
    }

    gceIndices.Resize(indices.size());
    size_t reverseIndice = indices.size() - 1;
    for (size_t i = 0; i < indices.size(); ++i)
        gceIndices[reverseIndice - i] = indices[i];

    customGeo = new gce::Geometry(gceVertices.Data(), gceVertices.Size(), gceIndices.Data(), gceIndices.Size());
    return customGeo;
}

gce::Vector3f32 ClosestPointOnLine(const gce::Vector3f32& a, const gce::Vector3f32& b, const gce::Vector3f32& p)
{

    gce::Vector3f32 d = b - a;
    gce::Vector3f32 ap = p - a;

    float denom = d.DotProduct(d);
    if (denom == 0.0f)  
        return a;

    float t = ap.DotProduct(d) / denom;

    t = max(0.f,min(1.f, t));

    return a + d * t;
}

bool IsPointNearLine(const gce::Vector3f32& a, const gce::Vector3f32& b, const gce::Vector3f32& p, float radius)
{
    gce::Vector3f32 d = b - a;
    gce::Vector3f32 ap = p - a;

    float denom = d.DotProduct(d);
    if (denom == 0.0f)  // a == b
        return ap.DotProduct(ap) <= radius * radius;

    float t = ap.DotProduct(d) / denom;

    if (t < 0.0f) t = 0.0f;
    else if (t > 1.0f) t = 1.0f;

    gce::Vector3f32 proj = a + d * t;
    gce::Vector3f32 diff = p - proj;

    return diff.DotProduct(diff) <= radius * radius;
}