#include"Scene.h"
#include"DxCore/DxCore.h"
#include"PostProcessing/CubeMap.h"
#include"Shader/Shader.h"
#include"Shader/CommonShaderRootSignature.h"
#include"PostProcessing/ShadowMap.h"
#include"PostProcessing/SSAO.h"
#include"JGLight.h"
#include"CommonData.h"
using namespace JGRC;
using namespace std;
using namespace DirectX;

Scene::Scene(class DxCore* core)
{
	m_DxCore = core;
	m_SceneConfig = core->GetSettingDesc();

	m_ResourceManager = make_unique<ResourceManager>();
	m_ResourceManager->Init(m_DxCore);

	InitStaticMemberVar();
	m_FrameResourceManager = make_unique<EngineFrameResourceManager>();
	m_MaterialCreater = make_unique<JGMaterialCreater>();
	m_LightManager = make_unique<LightManager>();
	

	// ���� �о� �߰�
	m_MainPass = AddPassData();

	// ī�޶� ��ġ
	auto MainCam = make_unique<Camera>();
	MainCam->SetLens(4.0f / XM_PI, core->AspectRatio(), core->GetSettingDesc().NearZ,
		core->GetSettingDesc().FarZ);
	MainCam->UpdateViewMatrix();
	m_MainCamera = MainCam.get();
	m_Cameras.push_back(move(MainCam));

	// ��� �� ����( ���߿� ũ�� ���� �����ϰ� ) ���߿� ��� ������Ʈ�� �Ⱦ �ڵ� ���� �÷Ǵ� �ٿ���.
	m_SceneSphere.Center = { 0.0f,0.0f, 0.0f };
	m_SceneSphere.Radius = sqrt(200.0f * 200.0f + 200.0f * 200.0f);
}
void Scene::BuildScene()
{
	CreateDebugObj();
	m_SSAO = make_unique<SSAO>();
	m_SSAO->BuildSSAO(m_SceneConfig.Width, m_SceneConfig.Height, m_DxCore->CommandList());


	// �Ӥ��� �׸��� PSO �����..
	m_LightManager->BuildLight();

	// ������Ʈ �и�
	for (auto& obj : m_ObjectMems)
	{
		obj->Build(m_DxCore->CommandList());
		EPSOMode Mode = obj->GetMaterial()->GetDesc()->Mode;
		if (Mode == EPSOMode::SKYBOX)
		{
			if (m_MainSkyBox)
				continue;
			else
				m_MainSkyBox = obj.get();
		}
		m_Objects[Mode].push_back(obj.get());
	}
	// �޽� ����
	for (auto& mesh : m_MeshMems)
		mesh->CreateMesh(m_DxCore->Device(), m_DxCore->CommandList());


	std::vector<wstring> CubeTex;
	// �ؽ��� �߰�
	for (auto& data : m_TextureDatas)
	{
		if (data.Type == ETextureType::Cube)
		{
			CubeTex.push_back(data.Path);
			m_TextureName = data.Path;
		}
			
		m_ResourceManager->AddTexture(m_DxCore->CommandList(), data.Path, data.Type);
	}
	// ���� ����Ʈ ���μ��� ����
	m_ResourceManager->BuildResourceHeap();
	m_MaterialCreater->BuildMaterial();

	m_LightManager->BuildShadowShader(m_MaterialCreater->GetCommonShaderRSClass());



	// �� ��� �� ���̴� ����
	m_SSAO->BuildSSAOShader(m_MaterialCreater->GetCommonShaderRSClass());



	// ��ī�� �ڽ� �ؽ��� �ڵ� ��� ����
	for (auto& sky : CubeTex)
	{
		UINT Index = m_ResourceManager->GetTextureIndex(sky);
		m_SkyCubeTextureHandles.push_back(m_ResourceManager->GetGPUSrvUavHandle(Index));
	}
	if (!CubeTex.empty())
	{
		m_MainSkyBoxHandle = &m_SkyCubeTextureHandles[0];
	}
	m_FrameResourceManager->BuildFrameResource(m_DxCore->Device(), 
		(UINT)max(1, m_PassDatas.size()), (UINT)max(1, m_ObjectMems.size() + 1),
		(UINT)max(1, m_MaterialCreater->Size()),
		(UINT)max(1, m_LightManager->Size()));
}
void Scene::OnReSize(UINT width, UINT height)
{
	m_SSAO->OnReSize(width, height);
}
void Scene::Update(const GameTimer& gt)
{
	// �� ���� ������Ʈ
	m_SceneConfig = m_DxCore->GetSettingDesc();

	// ���� ī�޶� ������Ʈ
	m_MainCamera->UpdateViewMatrix();
	m_FrameResourceManager->FrameResourcePerFrame(m_DxCore->Fence());
	auto CurrFrameResource = m_FrameResourceManager->CurrentFrameResource();
	// ������Ʈ ������Ʈ
	for (auto& obj : m_ObjectMems)
		obj->Update(gt, CurrFrameResource);
	m_DebugObj->Update(gt, CurrFrameResource);
	// ���͸��� ������Ʈ
	for (auto& mat : m_MaterialCreater->GetArray())
		mat.second->Update(CurrFrameResource);

	// �ӽ� �׸��� ������Ʈ
	m_LightManager->Update(CurrFrameResource);


	// ���� �н� ������Ʈ
	MainPassUpdate(gt);
	m_SSAO->Update(CurrFrameResource);
}
void Scene::Draw()
{
	// �� ��� ����Ʈ �� �޸� �ʱ�ȭ
	auto CommandList = m_DxCore->CommandList();
	auto CurrFrameResource = m_FrameResourceManager->CurrentFrameResource();
	auto CmdLisAlloc = CurrFrameResource->CmdListAlloc;
	ThrowIfFailed(CmdLisAlloc->Reset());
	ThrowIfFailed(CommandList->Reset(CmdLisAlloc.Get(), nullptr));

	/*
	Srv Heap, RootSignature, TextureArray, Material �� ��ɸ���Ʈ�� ��� */
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_ResourceManager->SrvHeap() };
	CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	CommandList->SetGraphicsRootSignature(m_MaterialCreater->GetRootSignature());
	CommandList->SetGraphicsRootDescriptorTable((UINT)ECommonShaderSlot::srvTexture,
		m_ResourceManager->SrvHeap()->GetGPUDescriptorHandleForHeapStart());

	auto MatCB = CurrFrameResource->MaterialCB->Resource();
	CommandList->SetGraphicsRootShaderResourceView((UINT)ECommonShaderSlot::sbMaterialData,
		MatCB->GetGPUVirtualAddress());

	auto LightCB = CurrFrameResource->LightCB->Resource();
	CommandList->SetGraphicsRootShaderResourceView((UINT)ECommonShaderSlot::sbLightData,
		LightCB->GetGPUVirtualAddress());


	// �о� ��
	m_LightManager->DrawShadowMap(CommandList, CurrFrameResource);


	// ��� �� �׸���
	m_SSAO->Draw(CurrFrameResource, CommandList);
	
	CommandList->SetGraphicsRootSignature(m_MaterialCreater->GetRootSignature());
	CommandList->SetGraphicsRootDescriptorTable((UINT)ECommonShaderSlot::srvTexture,
		m_ResourceManager->GetGPUTexture2DHandle());



	CommandList->SetGraphicsRootShaderResourceView((UINT)ECommonShaderSlot::sbMaterialData,
		MatCB->GetGPUVirtualAddress());

	// ť��� ���
	CommandList->SetGraphicsRootDescriptorTable(
		(int)ECommonShaderSlot::srvCubeMap, m_ResourceManager->GetGPUCubeMapHandle());


	// ť��� �׸���
	for (auto& obj : m_ObjectMems)
		obj->CubeMapDraw(CurrFrameResource, CommandList);

	// SwapChain�� �׸��� ����
	m_DxCore->StartDraw();

	// ���� �о� ���
	CommandList->SetGraphicsRootConstantBufferView((UINT)ECommonShaderSlot::cbPerPass,
		MainPassHandle());

	// �⺻ ������Ʈ �׸���
	for (auto& obj : m_Objects[EPSOMode::DEFAULT])
	{
		obj->Draw(CurrFrameResource, m_DxCore->CommandList());
	}
	//m_DebugObj->Draw(CurrFrameResource, m_DxCore->CommandList());

	for (auto& obj : m_Objects[EPSOMode::SKYBOX])
	{
		obj->Draw(CurrFrameResource, m_DxCore->CommandList());
	}

	// ������
	m_DxCore->EndDraw();
	CurrFrameResource->Fence = m_DxCore->CurrentFence();
}
JGRCObject* Scene::CreateObject()
{
	auto Obj = make_unique<JGRCObject>(++m_ObjIndex);
	JGRCObject* result = Obj.get();

	m_ObjectMems.push_back(move(Obj));
	return result;
}
JGMaterial* Scene::AddMaterial(const MaterialDesc& Desc)
{
	return m_MaterialCreater->CreateMaterial(Desc);
}
JGMesh*     Scene::AddMesh()
{
	auto Mesh = make_unique<JGMesh>("None");
	JGMesh* result = Mesh.get();
	m_MeshMems.push_back(move(Mesh));
	return result;
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
	auto passData = make_unique<PassData>();
	passData->PassCBIndex = ++m_PassCBIndex;
	PassData* result = passData.get();
	m_PassDatas.push_back(move(passData));
	return result;
}
JGLight*  Scene::AddLight(ELightType type)
{
	return m_LightManager->AddLight(type);
}
void Scene::AddTexture(const wstring& TexturePath, ETextureType type)
{
	m_TextureDatas.push_back(TexturePack{ move(TexturePath), nullptr, type });
}
UINT  Scene::GetSkyBoxShaderIndex()
{
	return m_ResourceManager->GetCubeTextureShaderIndex(m_TextureName);
}
D3D12_GPU_VIRTUAL_ADDRESS Scene::MainPassHandle()
{
	UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(cbPassConstant));
	D3D12_GPU_VIRTUAL_ADDRESS Handle = m_FrameResourceManager->CurrentFrameResource()->PassCB->Resource()->GetGPUVirtualAddress();
	Handle += m_MainPass->PassCBIndex * passCBByteSize;
	return Handle;
}

void Scene::CreateDebugObj()
{
	m_DebugObj = make_unique<JGRCObject>(++m_ObjIndex);
	MaterialDesc desc;
	desc.Name = "osaidjf";
	desc.Mode = EPSOMode::DEFAULT;
	desc.ShaderPath = L"../Contents/Engine/Shaders/ShadowDebug.hlsl";
	JGMaterial* debug = AddMaterial(desc);
	JGMesh* mesh = AddMesh();
	mesh->AddQuadArg("Debug", 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	m_DebugObj->SetMesh(mesh, "Debug");
	m_DebugObj->SetMaterial(debug);
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

	XMStoreFloat4x4(&m_MainPass->Data.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&m_MainPass->Data.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&m_MainPass->Data.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&m_MainPass->Data.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&m_MainPass->Data.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&m_MainPass->Data.InvViewProj, XMMatrixTranspose(invViewProj));
	XMStoreFloat4x4(&m_MainPass->Data.ViewProjTex, XMMatrixTranspose(viewProjTex));
	m_MainPass->Data.EyePosW = m_MainCamera->GetPosition3f();
	DxSetting set = m_DxCore->GetSettingDesc();
	m_MainPass->Data.RenderTargetSize = XMFLOAT2((float)set.Width, (float)set.Height);
	m_MainPass->Data.InvRenderTargetSize = XMFLOAT2(1 / (float)set.Width, 1 / (float)set.Height);
	m_MainPass->Data.NearZ = m_DxCore->GetSettingDesc().NearZ;
	m_MainPass->Data.FarZ = m_DxCore->GetSettingDesc().FarZ;
	m_MainPass->Data.TotalTime = gt.TotalTime();
	m_MainPass->Data.DeltaTime = gt.DeltaTime();
	m_MainPass->Data.AmbientLight = { 0.1f,0.12f,0.25f,1.0f };
	m_MainPass->Data.SSAOTexutreIndex = m_SSAO->GetShaderIndex();
	m_MainPass->Data.SkyBoxIndex = m_ResourceManager->GetCubeTextureShaderIndex(m_TextureName);
	auto PassCB = CurrFrameResource->PassCB.get();
	PassCB->CopyData(m_MainPass->PassCBIndex, m_MainPass->Data);
}
void Scene::InitStaticMemberVar()
{
	CommonData(m_DxCore, this, m_ResourceManager.get());
	CubeMap(512, 512);
	ShadowMap(2048, 2048);
}