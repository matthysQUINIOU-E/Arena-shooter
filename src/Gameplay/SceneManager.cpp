#include <Console.h>
#include "SceneManager.h"
#include <windows.h>
#include <Render.h>
#include <Engine.h>


SceneManager::SceneManager()
{
		Init();
}
void SceneManager::CreateWindowParam()
{
	m_pParam = new gce::WindowParam;
	m_pParam->width = 1920;
	m_pParam->height = 1080;
	m_pParam->title = L"Arena Shooter";
	m_pParam->isFullScreen = false;
	m_pParam->isSplitScreen = false;
}
void SceneManager::InitScene()
{
	gce::Scene& scene = gce::Scene::Create();

	gce::GameObject& cameraObject = gce::GameObject::Create(scene);
	cameraObject.transform.LocalTranslate({ 0, 0, -10 });

	m_pCamera = cameraObject.AddComponent<gce::Camera>();
	InitGamePlayScene(scene);
}

void SceneManager::InitGamePlayScene(gce::Scene& scene)
{

	static gce::D12PipelineObject s_defaultPso = {
   gce::SHADERS.VERTEX,
   gce::SHADERS.PIXEL,
   gce::SHADERS.HULL,
   gce::SHADERS.DOMAIN_,
   gce::SHADERS.ROOT_SIGNATURE
	};
	gce::LightManager::SetLightsProperties(8.0f, 100.0f, 2.0f, 32.0f, 1.f);
	gce::LightData directionalLight = gce::LightManager::CreateDirectionalLight(gce::Vector3f32(0.0f, 0.0f, 1.0f), gce::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
	gce::LightManager::AddLight(directionalLight);
	//Creation d'un objet sans texture
	gce::GameObject& testObject = gce::GameObject::Create(scene);
	gce::MeshRenderer* pMeshRenderer = testObject.AddComponent<gce::MeshRenderer>();
	// SHAPES contient plusieurs shapes de base (je vous laisse regarder) pour faire des formes de bases
	pMeshRenderer->pGeometry = gce::SHAPES.CUBE;
	pMeshRenderer->pPso = &s_defaultPso;
	m_SceneObjectsList[Scene::GamePlayScene].push_back(&testObject);
	gce::Texture* pNewTexture = new gce::Texture("res/Exemple/mogwai_lowpoly_texture.png");
	gce::GameObject& shapeCustom = gce::GameObject::Create(scene);
	shapeCustom.transform.SetWorldPosition({ 0.0f,0.0f,-6.0f });
	gce::MeshRenderer* pMeshRenderer3 = shapeCustom.AddComponent<gce::MeshRenderer>();
	// Pensez à inverser l'axe Y des uv quand vous loadez des OBJ (sinon ça casse vos textures)
shapeCustom.transform.SetLocalRotation(gce::Vector3f32{ shapeCustom.transform.GetLocalRotation().GetX(),shapeCustom.transform.GetLocalRotation().GetY() +.5f,shapeCustom.transform.GetLocalRotation().GetZ() });
	pMeshRenderer3->pGeometry = gce::GeometryFactory::LoadGeometry("res/Exemple/mogwai_lowpoly.obj");
	pMeshRenderer3->pPso = &s_defaultPso;
	pMeshRenderer3->pMaterial->albedoTextureID = pNewTexture->GetTextureID();
	pMeshRenderer3->pMaterial->useTextureAlbedo = 1;
	pMeshRenderer3->pMaterial->subsurface = 1;
}

void SceneManager::InitCamera()
{
	m_pCamera->SetMainCamera();
	m_pCamera->SetType(gce::PERSPECTIVE);

	m_pCamera->perspective.fov = XM_PIDIV4;
	m_pCamera->perspective.nearPlane = 0.001f;
	m_pCamera->perspective.farPlane = 500.0f;
	m_pCamera->perspective.aspectRatio = 1000.0f / 800.0f;
	m_pCamera->perspective.up = { 0.0f, 1.0f, 0.0f };
}
void SceneManager::Init()
{
	gce::GameManager::Create();
	gce::Console::Init();
	InitScene();
	InitCamera();
	CreateWindowParam();
	gce::GameManager::Run(*m_pParam);

}

