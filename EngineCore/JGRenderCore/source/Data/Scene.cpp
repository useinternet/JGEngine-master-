#include"Scene.h"
#include"DxCore/DxDevice.h"
#include"SceneData.h"
#include"Shader/Shader.h"
#include"Shader/CommonShaderRootSignature.h"
#include"Debug/DebugBox.h"
#include"PostProcessing/ShadowMap.h"
#include"PostProcessing/BlurFilter.h"
#include"PostProcessing/SSAO.h"

#include"JGLight.h"
#include"Object/SceneObject.h"
#include"Mesh/JGStaticMesh.h"
#include"Mesh/JGSkeletalMesh.h"
#include"LightManager.h"
#include"JGMaterial.h"
#include"CommonData.h"
#include"ResourceManagement/ResourceReader.h"
#include"ResourceManagement/DataManager.h"
#include"Debug/DebugBox.h"
#include"DxCore/CommandListManager.h"
#include"DxCore/GpuCpuSynchronizer.h"
#include"DxCore/ScreenManager.h"
#include"DxCore/RootSignatureManager.h"
using namespace JGRC;
using namespace std;
using namespace DirectX;

Scene::Scene(const string& name, const SceneConfig& config)
{
	m_Name = name;
	m_DxDevice         = CommonData::_DxDevice();
	m_ResourceManager = CommonData::_ResourceManager();
	m_CmdListManager  = CommonData::_CmdListManager();
	m_ScreenManager   = CommonData::_ScreenManager();
	m_SceneConfig     = config;
	// �� �� ������ ���� ��� �ʱ�ȭ
	ShadowMap(config.ShadowWidth, config.ShadowHeight);
	m_SceneData = make_unique<SceneData>();
	m_LightManager = make_unique<LightManager>();


	Shader SceneShader(global_scene_hlsl_path, { EShaderType::Vertex, EShaderType::Pixel });
	m_ScenePSO = SceneShader.CompileAndConstrutPSO(
		EPSOMode::SCENE, CommonData::_RootSigManager()->GetRootSig(ERootSigType::Common));

	// ���� �о� �߰�
	m_MainPass = AddPassData();

	// ī�޶� ��ġ
	auto MainCam = make_unique<Camera>();
	MainCam->SetLens(4.0f / XM_PI,
		(float)m_SceneConfig.Width / (float)m_SceneConfig.Height,
		m_SceneConfig.NearZ,
		m_SceneConfig.FarZ);

	MainCam->UpdateViewMatrix();
	m_MainCamera = MainCam.get();
	m_Cameras.push_back(move(MainCam));


	// ��� �� ����( ���߿� ũ�� ���� �����ϰ� ) ���߿� ��� ������Ʈ�� �Ⱦ �ڵ� ���� �÷Ǵ� �ٿ���.
	m_SceneSphere.Center = { 0.0f,0.0f, 0.0f };
	m_SceneSphere.Radius = sqrt(500.0f * 500.0f + 500.0f * 500.0f);
}
void Scene::BuildScene()
{
	auto CommandList = m_CmdListManager->GetCommandList(0);

	// �� ������ ����
	m_SceneData->BuildSceneData(m_SceneConfig.Width, m_SceneConfig.Height);
	m_LightManager->BuildLight();
	// ssao ����
	m_SSAO = make_unique<SSAO>();
	m_SSAO->BuildSSAO(m_SceneConfig.Width, m_SceneConfig.Height, CommandList);
	m_Blur = make_unique<BlurFilter>(m_SceneConfig.Width, m_SceneConfig.Height);
	// ������Ʈ �и�
	for (auto& obj : m_ObjectMems)
	{
		EPSOMode Mode = obj->GetMaterial()->GetDesc()->Mode;
		EObjectType Type = obj->GetType();
		m_ObjectArray[Type][Mode].push_back(obj);
	}
	// �ؽ��� �߰�
	for (auto& data : m_TextureDatas)
	{
		m_ResourceManager->AddTexture(CommandList, data.Path, data.Type);
	}
}
void Scene::OnReSize(UINT width, UINT height)
{
	m_SceneData->ReSize(width, height);
	m_SSAO->OnReSize(width, height);
}
void Scene::Update(const GameTimer& gt)
{
	// ���� ī�޶� ������Ʈ
	m_MainCamera->UpdateViewMatrix();
	auto CurrFrameResource = CommonData::_EngineFrameResourceManager()->CurrentFrameResource();
	m_SSAO->Update(CurrFrameResource);
	// ������Ʈ ������Ʈ
	for (auto& obj : m_ObjectMems)
		obj->Update(gt, CurrFrameResource);
	// ���͸��� ������Ʈ
	for (auto& mat : m_MaterialMems)
		mat->Update(CurrFrameResource);

	// ����Ʈ �Ŵ��� 
	m_LightManager->Update(CurrFrameResource);

	// ���� �н� ������Ʈ
	MainPassUpdate(gt);

}
void Scene::Draw()
{
	// �� ��� ����Ʈ �� �޸� �ʱ�ȭ
	auto CommandList = m_CmdListManager->GetCommandList(0);
	auto CurrFrameResource = CommonData::_EngineFrameResourceManager()->CurrentFrameResource();
	auto CmdLisAlloc = CurrFrameResource->CmdListAlloc;
	ThrowIfFailed(CmdLisAlloc->Reset());
	ThrowIfFailed(CommandList->Reset(CmdLisAlloc.Get(), nullptr));

	/*
	Srv Heap, RootSignature, TextureArray, Material �� ��ɸ���Ʈ�� ��� */
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_ResourceManager->SrvHeap() };
	CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);


	auto RootSig = CommonData::_RootSigManager()->GetRootSig(ERootSigType::Common);
	CommandList->SetGraphicsRootSignature(RootSig->Get());
	CommandList->SetGraphicsRootDescriptorTable((UINT)ECommonShaderSlot::srvTexture,
		m_ResourceManager->SrvHeap()->GetGPUDescriptorHandleForHeapStart());
	CommandList->SetGraphicsRootDescriptorTable((UINT)ECommonShaderSlot::srvCubeMap,
		m_ResourceManager->GetGPUCubeMapHandle());
	auto MatCB = CurrFrameResource->MaterialCB->Resource();
	CommandList->SetGraphicsRootShaderResourceView((UINT)ECommonShaderSlot::sbMaterialData,
		MatCB->GetGPUVirtualAddress());
	auto LightCB = CurrFrameResource->LightCB->Resource();
	CommandList->SetGraphicsRootShaderResourceView((UINT)ECommonShaderSlot::sbLightData,
		LightCB->GetGPUVirtualAddress());


	m_SSAO->Draw(CurrFrameResource, CommandList);





	CommandList->SetGraphicsRootSignature(RootSig->Get());
	CommandList->SetGraphicsRootDescriptorTable((UINT)ECommonShaderSlot::srvTexture,
		m_ResourceManager->SrvHeap()->GetGPUDescriptorHandleForHeapStart());
	CommandList->SetGraphicsRootDescriptorTable((UINT)ECommonShaderSlot::srvCubeMap,
		m_ResourceManager->GetGPUCubeMapHandle());
	CommandList->SetGraphicsRootShaderResourceView((UINT)ECommonShaderSlot::sbMaterialData,
		MatCB->GetGPUVirtualAddress());
	CommandList->SetGraphicsRootShaderResourceView((UINT)ECommonShaderSlot::sbLightData,
		LightCB->GetGPUVirtualAddress());


	m_LightManager->DrawShadowMap(CommandList, CurrFrameResource);

	// ���� �о� ���
	CommandList->SetGraphicsRootConstantBufferView((UINT)ECommonShaderSlot::cbPerPass,
		CurrFrameResource->PassCBHeapAddress(m_MainPass));
	m_SceneData->SceneDataExtract(CurrFrameResource, CommandList);


	// SwapChain�� �׸��� ����
	m_ScreenManager->ReadyScreen(m_Name, CommandList);

	CommandList->SetPipelineState(m_ScenePSO.Get());
	CommandList->IASetVertexBuffers(0, 0, nullptr);
	CommandList->IASetIndexBuffer(nullptr);
	CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CommandList->DrawInstanced(6, 1, 0, 0);

	//m_Blur->Execute(m_DxDevice->CurrentBackBuffer(), CommandList, D3D12_RESOURCE_STATE_RENDER_TARGET, 1);
	// ������
	m_ScreenManager->OutputScreen(m_Name, CommandList);
}
void Scene::SceneObjectDraw(ID3D12GraphicsCommandList* CommandList, FrameResource* CurrFrameResource, EObjectRenderMode Mode, bool bDebug)
{
	for (auto& objarr : m_ObjectArray)
	{
		for (auto& obj : objarr.second[EPSOMode::DEFAULT])
		{
			obj->Draw(CurrFrameResource, CommandList, Mode);
		}
		if (bDebug)
		{
			for (auto& obj : objarr.second[EPSOMode::DEBUG])
			{
				obj->Draw(CurrFrameResource, CommandList, Mode);
			}
		}
	}

}
void Scene::SceneStaticObjectDraw(ID3D12GraphicsCommandList* CommandList, FrameResource* CurrFrameResource, EObjectRenderMode Mode)
{
	for (auto& obj : m_ObjectArray[EObjectType::Static][EPSOMode::DEFAULT])
	{
		obj->Draw(CurrFrameResource, CommandList, Mode);
	}
}
void Scene::SceneDynamicObjectDraw(ID3D12GraphicsCommandList* CommandList, FrameResource* CurrFrameResource, EObjectRenderMode Mode)
{
	for (auto& obj : m_ObjectArray[EObjectType::Dynamic][EPSOMode::DEFAULT])
	{
		obj->Draw(CurrFrameResource, CommandList, Mode);
	}
}
SceneObject* Scene::CreateObject(const string& name, JGMaterial* mat, const string& matDataName, JGBaseMesh* mesh, const string& meshname, EObjectType Type)
{
	SceneObject* result = CommonData::_DataManager()->CreateSceneObject(name, mat, matDataName, mesh, meshname, Type);
	m_ObjectMems.push_back(result);
	return result;
}
SceneObject* Scene::CreateSkyBox(const std::wstring& texturepath)
{
	bool result = false;
	//��ī�� �ڽ� ���͸��� �� �޽� �̸� ����
	for (auto& path : m_TextureDatas)
		if (texturepath == path.Path)
		{
			result = true;
			break;
		}
	if (!result)
		return nullptr;


	// ���͸��� ����
	MaterialDesc matDesc;
	matDesc.Mode = EPSOMode::SKYBOX;
	matDesc.ShaderPath = global_sky_hlsl_path;
	matDesc.Name = string().assign(m_SkyShaderPath.begin(), m_SkyShaderPath.end());
	JGMaterial* SkyMat = AddMaterial(matDesc);
	MatPersonalData* SkyMatData = SkyMat->AddData("SkyMatData");
	SkyMat->SetTexture(ETextureSlot::Diffuse, texturepath);
	// �޽� ����
	JGStaticMesh* SkyMesh = AddStaticMesh("SkyBoxMesh");
	SkyMesh->AddBoxArg("Scene_SkyBox_Default_Mesh", 1.0f, 1.0f, 1.0f, 0);

	// ������Ʈ ����
	SceneObject* obj = CreateObject("SkyBox", SkyMat, SkyMatData->Name, SkyMesh,"Scene_SkyBox_Default_Mesh", EObjectType::Static );
	obj->GetScale().Set(5000.0f, 5000.0f, 5000.0f);
	return obj;
}
void   Scene::AddDebugBox(SceneObject* obj, const XMFLOAT3& color, float thickness)
{
	auto debugBox = make_unique<DebugBox>();
	debugBox->BindingObject(obj, color, thickness);
	m_DebugMems.push_back(move(debugBox));
}
JGMaterial* Scene::AddMaterial(const MaterialDesc& Desc)
{
	JGMaterial* mat  = CommonData::_DataManager()->CreateMaterial(Desc);
	m_MaterialMems.push_back(mat);
	return mat;
}
JGStaticMesh*   Scene::AddStaticMesh(const std::string& name)
{
	return CommonData::_DataManager()->CreateStaticMesh(name);
}
JGSkeletalMesh* Scene::AddSkeletalMesh(const std::string& name)
{
	return CommonData::_DataManager()->CreateSkeletalMesh(name);
}
string Scene::AddAnimation(const string& path)
{
	if (m_Animations.find(path) != m_Animations.end())
		return string("None");


	auto anim = make_unique<JGAnimation>();
	ResourceReader reader(path, *anim);
	JGAnimation* result = anim.get();

	m_AnimationMems.push_back(move(anim));
	m_Animations[path] = result;
	return path;
}
Camera*     Scene::AddCamera()
{
	auto cam = make_unique<Camera>();
	Camera* result = cam.get();
	m_Cameras.push_back(move(cam));
	return result;
}
PassData* Scene::AddPassData()
{
	return CommonData::_DataManager()->AddPassData();
}
JGLight*  Scene::AddLight(ELightType type, ELightExercise extype)
{
	return m_LightManager->AddLight(type, extype);
}
void Scene::AddTexture(const wstring& TexturePath, ETextureType type)
{
	m_TextureDatas.push_back(TexturePack{ move(TexturePath), nullptr, type });
}
SceneObject*  Scene::GetMainSkyBox() const
{
	return m_MainSkyBox;
}
void         Scene::SetMainSkyBox(SceneObject* skybox)
{
	if(skybox->GetMaterial()->GetDesc()->Mode == EPSOMode::SKYBOX) 	
		m_MainSkyBox = skybox;
}
JGAnimation* Scene::GetAnimation(const string& name)
{
	if (m_Animations.find(name) == m_Animations.end())
		return nullptr;
	return m_Animations[name];
}
SceneData* Scene::GetSceneData() const
{
	return m_SceneData.get();
}
LightManager* Scene::GetLightManager()
{
	return m_LightManager.get();
}
void Scene::MainPassUpdate(const GameTimer& gt)
{
	auto CurrFrameResource = CommonData::_EngineFrameResourceManager()->CurrentFrameResource();
	XMMATRIX view = m_MainCamera->GetView();
	XMMATRIX proj = m_MainCamera->GetProj();
	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
	XMMATRIX viewProjTex = XMMatrixMultiply(viewProj, T);
	// �⺻ ��� 
	XMStoreFloat4x4(&m_MainPass->Get().View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&m_MainPass->Get().Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&m_MainPass->Get().ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&m_MainPass->Get().InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&m_MainPass->Get().InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&m_MainPass->Get().InvViewProj, XMMatrixTranspose(invViewProj));
	XMStoreFloat4x4(&m_MainPass->Get().ViewProjTex, XMMatrixTranspose(viewProjTex));

	// ī�޶� ��ġ
	m_MainPass->Get().EyePosW = m_MainCamera->GetPosition3f();


	// ���� Ÿ�� ������
	m_MainPass->Get().RenderTargetSize = XMFLOAT2((float)m_SceneConfig.Width, (float)m_SceneConfig.Height);
	m_MainPass->Get().InvRenderTargetSize = XMFLOAT2(1 / (float)m_SceneConfig.Width, 1 / (float)m_SceneConfig.Height);

	//
	m_MainPass->Get().TotalTime = gt.TotalTime();
	m_MainPass->Get().DeltaTime = gt.DeltaTime();
	// �� �ε���
	m_MainPass->Get().WorldPosSceneIndex = m_SceneData->GetWorldPosIndex();
	m_MainPass->Get().AlbedoSceneIndex = m_SceneData->GetAlbedoIndex();
	m_MainPass->Get().NormalSceneIndex = m_SceneData->GetNormalIndex();
	m_MainPass->Get().DepthSceneIndex = m_SceneData->GetDepthIndex();
	m_MainPass->Get().MatSceneIndex = m_SceneData->GetMatIndex();
	m_MainPass->Get().SSAOTexutreIndex = m_SSAO->GetShaderIndex();
	if(m_MainSkyBox)
		m_MainPass->Get().SkyBoxIndex = m_ResourceManager->GetCubeTextureShaderIndex(
			m_MainSkyBox->GetMaterial()->GetTexturePath(ETextureSlot::Diffuse));
	auto PassCB = CurrFrameResource->PassCB.get();
	PassCB->CopyData(m_MainPass->Index(), m_MainPass->Get());
}