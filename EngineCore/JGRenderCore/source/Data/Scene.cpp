#include"Scene.h"
#include"DxCore/DxCore.h"
#include"SceneData.h"
#include"Shader/Shader.h"
#include"Shader/CommonShaderRootSignature.h"

#include"PostProcessing/ShadowMap.h"
#include"PostProcessing/BlurFilter.h"
#include"PostProcessing/SSAO.h"

#include"JGLight.h"
#include"CommonData.h"
#include"ResourceManagement/ResourceReader.h"
#include"Debug/DebugBox.h"
#include"DxCore/CommandListManager.h"
#include"DxCore/GpuCpuSynchronizer.h"
#include"DxCore/ScreenManager.h"
using namespace JGRC;
using namespace std;
using namespace DirectX;

Scene::Scene(const string& name,
	DxCore* core, ResourceManager* manager, CommandListManager* cmdManager,
	ScreenManager* screenManager, GpuCpuSynchronizer* gcs)
{
	m_Name = name;
	m_DxCore = core;
	m_ResourceManager = manager;
	m_CmdListManager = cmdManager;
	m_ScreenManager = screenManager;
	m_GCS = gcs;


	m_SceneConfig = core->GetSettingDesc();
	// �� �� ������ ���� ��� �ʱ�ȭ
	InitStaticMemberVar();
	// ������ ���ҽ��� ���͸��� ������ �ʱ�ȭ
	m_FrameResourceManager = make_unique<EngineFrameResourceManager>();

	// �������� �ʱ�ȭ �� ��Ʈ���� / �� ���̴�, pso ������ 
	m_SceneData = make_unique<SceneData>();
	m_CommonShaderRootSig = make_unique<CommonShaderRootSignature>();
	m_CommonShaderRootSig->RootSign(m_DxCore->Device());
	m_CommonSkinnedShadeRootSig = make_unique<CommonSkinnedShaderRootSignature>();
	m_CommonSkinnedShadeRootSig->RootSign(m_DxCore->Device());



	Shader SceneShader(global_scene_hlsl_path, { EShaderType::Vertex, EShaderType::Pixel });

	m_ScenePSO = SceneShader.CompileAndConstrutPSO(EPSOMode::SCENE, m_CommonShaderRootSig.get());



	m_LightManager = make_unique<LightManager>();


	// ���� �о� �߰�
	m_MainPass = AddPassData();

	// ī�޶� ��ġ
	auto MainCam = make_unique<Camera>();
	MainCam->SetLens(4.0f / XM_PI,
		(float)m_SceneConfig.Width / (float)m_SceneConfig.Height,
		core->GetSettingDesc().NearZ,
		core->GetSettingDesc().FarZ);

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
	m_LightManager->BuildLight(m_CommonShaderRootSig.get());
	// ssao ����
	m_SSAO = make_unique<SSAO>();
	m_SSAO->BuildSSAO(m_SceneConfig.Width, m_SceneConfig.Height,
		CommandList, m_CommonShaderRootSig.get());
	m_Blur = make_unique<BlurFilter>(m_SceneConfig.Width, m_SceneConfig.Height);
	// ������Ʈ �и�
	for (auto& obj : m_ObjectMems)
	{
		EPSOMode Mode = obj->GetMaterial()->GetDesc()->Mode;
		EObjType Type = obj->GetType();

		m_ObjectArray[Type][Mode].push_back(obj);
	}
	// �ؽ��� �߰�
	for (auto& data : m_TextureDatas)
	{
		m_ResourceManager->AddTexture(CommandList, data.Path, data.Type);
	}
	// ���� ����Ʈ ���μ��� ����
	m_ResourceManager->BuildResourceManager(CommandList, m_CommonShaderRootSig.get());


	m_FrameResourceManager->BuildFrameResource(m_DxCore->Device(), 
		(UINT)max(1, m_ResourceManager->PassDataSize()), (UINT)max(1, m_ResourceManager->JGRCObjectSize() + 1),
		(UINT)max(1, m_ResourceManager->JGMaterialSize()),
		(UINT)max(1, m_LightManager->Size()));
}
void Scene::OnReSize(UINT width, UINT height)
{
	m_SceneData->ReSize(width, height);
	m_SSAO->OnReSize(width, height);
}
void Scene::Update(const GameTimer& gt)
{
	// �� ���� ������Ʈ
	m_SceneConfig = m_DxCore->GetSettingDesc();

	// ���� ī�޶� ������Ʈ
	m_MainCamera->UpdateViewMatrix();

	m_FrameResourceManager->FrameResourcePerFrame(m_GCS->GetFence());

	auto CurrFrameResource = m_FrameResourceManager->CurrentFrameResource();
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
	auto CurrFrameResource = m_FrameResourceManager->CurrentFrameResource();
	auto CmdLisAlloc = CurrFrameResource->CmdListAlloc;
	ThrowIfFailed(CmdLisAlloc->Reset());
	ThrowIfFailed(CommandList->Reset(CmdLisAlloc.Get(), nullptr));

	/*
	Srv Heap, RootSignature, TextureArray, Material �� ��ɸ���Ʈ�� ��� */
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_ResourceManager->SrvHeap() };
	CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);



	CommandList->SetGraphicsRootSignature(m_CommonShaderRootSig->GetRootSignature());
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





	CommandList->SetGraphicsRootSignature(m_CommonShaderRootSig->GetRootSignature());
	CommandList->SetGraphicsRootDescriptorTable((UINT)ECommonShaderSlot::srvTexture,
		m_ResourceManager->SrvHeap()->GetGPUDescriptorHandleForHeapStart());
	CommandList->SetGraphicsRootDescriptorTable((UINT)ECommonShaderSlot::srvCubeMap,
		m_ResourceManager->GetGPUCubeMapHandle());
	CommandList->SetGraphicsRootShaderResourceView((UINT)ECommonShaderSlot::sbMaterialData,
		MatCB->GetGPUVirtualAddress());
	CommandList->SetGraphicsRootShaderResourceView((UINT)ECommonShaderSlot::sbLightData,
		LightCB->GetGPUVirtualAddress());


	m_LightManager->DrawShadowMap(CommandList, CurrFrameResource);
	


	for (auto& objarr : m_ObjectArray)
	{
		for (auto& obj : objarr.second[EPSOMode::DEFAULT])
		{
			obj->CubeMapDraw(CurrFrameResource, CommandList);
		}
	}



	// ���� �о� ���
	CommandList->SetGraphicsRootConstantBufferView((UINT)ECommonShaderSlot::cbPerPass,
		MainPassHandle());
	m_SceneData->SceneDataExtract(CurrFrameResource, CommandList);


	// SwapChain�� �׸��� ����
	m_ScreenManager->ReadyScreen(m_Name, CommandList);

	CommandList->SetPipelineState(m_ScenePSO.Get());
	CommandList->IASetVertexBuffers(0, 0, nullptr);
	CommandList->IASetIndexBuffer(nullptr);
	CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CommandList->DrawInstanced(6, 1, 0, 0);
	//m_Blur->Execute(m_DxCore->CurrentBackBuffer(), CommandList, D3D12_RESOURCE_STATE_RENDER_TARGET, 1);
	// ������
	m_ScreenManager->OutputScreen(m_Name, CommandList);

	CurrFrameResource->Fence = m_GCS->GetOffset();
}
void Scene::SceneObjectDraw(ID3D12GraphicsCommandList* CommandList, FrameResource* CurrFrameResource, EObjRenderMode Mode, bool bDebug)
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
void Scene::SceneStaticObjectDraw(ID3D12GraphicsCommandList* CommandList, FrameResource* CurrFrameResource, EObjRenderMode Mode)
{
	for (auto& obj : m_ObjectArray[EObjType::Static][EPSOMode::DEFAULT])
	{
		obj->Draw(CurrFrameResource, CommandList, Mode);
	}
}
void Scene::SceneDynamicObjectDraw(ID3D12GraphicsCommandList* CommandList, FrameResource* CurrFrameResource, EObjRenderMode Mode)
{
	for (auto& obj : m_ObjectArray[EObjType::Dynamic][EPSOMode::DEFAULT])
	{
		obj->Draw(CurrFrameResource, CommandList, Mode);
	}
}
JGRCObject* Scene::CreateObject(JGMaterial* mat, JGBaseMesh* mesh, const string& meshname, EObjType Type)
{
	JGRCObject* result = m_ResourceManager->AddJGRCObject(mat, mesh, meshname, Type);
	m_ObjectMems.push_back(result);
	return result;
}
JGRCObject* Scene::CreateSkyBox(const std::wstring& texturepath)
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
	SkyMat->SetTexture(ETextureSlot::Diffuse, texturepath);
	// �޽� ����
	JGStaticMesh* SkyMesh = AddStaticMesh();
	SkyMesh->AddBoxArg("Scene_SkyBox_Default_Mesh", 1.0f, 1.0f, 1.0f, 0);
	// ������Ʈ ����
	JGRCObject* obj = CreateObject(SkyMat, SkyMesh,"Scene_SkyBox_Default_Mesh" );
	obj->SetScale(5000.0f, 5000.0f, 5000.0f);
	return obj;
}
void   Scene::AddDebugBox(JGRCObject* obj, const XMFLOAT3& color, float thickness)
{
	auto debugBox = make_unique<DebugBox>();
	debugBox->BindingObject(obj, color, thickness);
	m_DebugMems.push_back(move(debugBox));
}
JGMaterial* Scene::AddMaterial(const MaterialDesc& Desc)
{
	JGMaterial* mat  = m_ResourceManager->AddMaterial(Desc);
	m_MaterialMems.push_back(mat);
	return mat;
}
JGStaticMesh*   Scene::AddStaticMesh()
{
	return m_ResourceManager->AddStaticMesh();
}
JGSkeletalMesh* Scene::AddSkeletalMesh()
{
	return m_ResourceManager->AddSkeletalMesh();
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
	return m_ResourceManager->AddPassData();
}
JGLight*  Scene::AddLight(ELightType type, ELightExercise extype)
{
	return m_LightManager->AddLight(type, extype);
}
void Scene::AddTexture(const wstring& TexturePath, ETextureType type)
{
	m_TextureDatas.push_back(TexturePack{ move(TexturePath), nullptr, type });
}
void  Scene::SettingDefaultSceneBuffer(ID3D12GraphicsCommandList* CommandList, FrameResource* CurrFrameResource)
{
	CommandList->SetGraphicsRootSignature(m_CommonShaderRootSig->GetRootSignature());
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
CommonShaderRootSignature* Scene::GetRootSig()
{
	return m_CommonShaderRootSig.get();
}
CommonSkinnedShaderRootSignature* Scene::GetSkinnedRootSig()
{
	return m_CommonSkinnedShadeRootSig.get();
}
D3D12_GPU_VIRTUAL_ADDRESS Scene::MainPassHandle()
{
	UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(cbPassConstant));
	D3D12_GPU_VIRTUAL_ADDRESS Handle = m_FrameResourceManager->CurrentFrameResource()->PassCB->Resource()->GetGPUVirtualAddress();
	Handle += m_MainPass->PassCBIndex * passCBByteSize;
	return Handle;
}
std::vector<JGRCObject*>& Scene::GetArray(EObjType objType, EPSOMode mode)
{
	return m_ObjectArray[objType][mode];
}
void Scene::MainPassUpdate(const GameTimer& gt)
{
	auto CurrFrameResource = m_FrameResourceManager->CurrentFrameResource();
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
	XMStoreFloat4x4(&m_MainPass->Data.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&m_MainPass->Data.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&m_MainPass->Data.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&m_MainPass->Data.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&m_MainPass->Data.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&m_MainPass->Data.InvViewProj, XMMatrixTranspose(invViewProj));
	XMStoreFloat4x4(&m_MainPass->Data.ViewProjTex, XMMatrixTranspose(viewProjTex));

	// ī�޶� ��ġ
	m_MainPass->Data.EyePosW = m_MainCamera->GetPosition3f();


	// ���� Ÿ�� ������
	m_MainPass->Data.RenderTargetSize = XMFLOAT2((float)m_SceneConfig.Width, (float)m_SceneConfig.Height);
	m_MainPass->Data.InvRenderTargetSize = XMFLOAT2(1 / (float)m_SceneConfig.Width, 1 / (float)m_SceneConfig.Height);

	//
	m_MainPass->Data.TotalTime = gt.TotalTime();
	m_MainPass->Data.DeltaTime = gt.DeltaTime();
	// �� �ε���
	m_MainPass->Data.WorldPosSceneIndex = m_SceneData->GetWorldPosIndex();
	m_MainPass->Data.AlbedoSceneIndex = m_SceneData->GetAlbedoIndex();
	m_MainPass->Data.NormalSceneIndex = m_SceneData->GetNormalIndex();
	m_MainPass->Data.DepthSceneIndex = m_SceneData->GetDepthIndex();
	m_MainPass->Data.MatSceneIndex = m_SceneData->GetMatIndex();
	m_MainPass->Data.SSAOTexutreIndex = m_SSAO->GetShaderIndex();
	if(m_MainSkyBox)
		m_MainPass->Data.SkyBoxIndex = m_ResourceManager->GetCubeTextureShaderIndex(
			m_MainSkyBox->GetMaterial()->GetTexturePath(ETextureSlot::Diffuse));
	auto PassCB = CurrFrameResource->PassCB.get();
	PassCB->CopyData(m_MainPass->PassCBIndex, m_MainPass->Data);
}
void Scene::InitStaticMemberVar()
{
	CommonData(m_DxCore, this, m_ResourceManager, m_ScreenManager);
	ShadowMap(2048, 2048);
}