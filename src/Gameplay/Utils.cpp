#include <Engine.h>
#include <fstream>
#include "json.h"

void ImportBlenderScene()
{
	const std::string pathTexture = "";
	const std::string pathObj = "";
	
	gce::GameManager* pGameManager = gce::GameManager::s_pInstance;
	gce::Scene* pScene = pGameManager->m_scenes[0]; // TODO :: changer la manière dont récupérer la scene avec le SceneManager

	std::ifstream f(WRES_PATH L"res/BlenderScene/blender_scene_v3.json");
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

        // Position
        gce::Vector3f32 position;
        if (obj.contains("position"))
        {
            position.x = obj["position"][0];
            position.y = obj["position"][1];
            position.z = obj["position"][2];
        }

        // Rotation (quaternion)
        gce::Quaternion rotation;
        if (obj.contains("rotation"))
            rotation = gce::Quaternion{ obj["rotation"][0], obj["rotation"][1], obj["rotation"][2], obj["rotation"][3] };
        else
            rotation = gce::Quaternion{ 0.f, 0.f, 0.f, 0.f };

        // Scale
        gce::Vector3f32 scale;
        if (obj.contains("scale"))
            scale = gce::Vector3f32{ obj["scale"][0], obj["scale"][1], obj["scale"][2] };
        else
            scale = gce::Vector3f32{ 1.f, 1.f, 1.f };

        // -------------------------------------
        // 2. Lecture du Mesh
        // -------------------------------------
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

        // -------------------------------------
        // 3. Lecture des textures (si présentes)
        // -------------------------------------
        std::string baseColorTex = "";
        std::string normalMapTex = "";

        if (obj.contains("textures"))
        {
            auto& tex = obj["textures"];

            if (tex.contains("base_color") && !tex["base_color"].is_null())
                baseColorTex = tex["base_color"].get<std::string>();

            if (tex.contains("normal_map") && !tex["normal_map"].is_null())
                normalMapTex = tex["normal_map"].get<std::string>();
        }

        // -------------------------------------
        // 4. Création de l’objet dans ton moteur
        // -------------------------------------


        gce::GameObject& gameObject = gce::GameObject::Create(*pScene); // TODO :: tag later

        gameObject.transform.SetWorldPosition(position);
        gameObject.transform.SetWorldRotation(rotation);
        gameObject.transform.SetWorldScale(scale);

        // Ajouter mesh renderer
        if (!vertices.empty())
        {
            gce::MeshRenderer* pMeshRenderer = gameObject.AddComponent<gce::MeshRenderer>();
            pMeshRenderer->pGeometry = new Geometry(Vertex const* vertex, uint64 vertexCount, uint32 const* indices, uint64 indicesCount);
            m->SetVertices(vertices);
            m->SetIndices(indices);
            m->SetUVs(uvs);

            go->AddComponent<mce::MeshRenderer>(m);
        }

        // Ajouter textures
        if (!baseColorTex.empty())
        {
            go->GetComponent<mce::MeshRenderer>()->SetTexture(pathTexture + baseColorTex);
        }

        if (!normalMapTex.empty())
        {
            go->GetComponent<mce::MeshRenderer>()->SetNormalMap(pathTexture + normalMapTex);
        }

        // Collider / Physic
        if (hasCollider)
            go->AddComponent<gce::Collider>();

        if (hasPhysic)
            go->AddComponent<gce::RigidBody>();

        std::cout << "Object imported: " << name << std::endl;
    }
}