#include"Scene.h"
#include"DxCore/DxCore.h"
#include"SceneData.h"
#include"PostProcessing/CubeMap.h"
#include"Shader/Shader.h"
#include"Shader/CommonShaderRootSignature.h"
#include"Shader/ShaderPath.h"
#include"PostProcessing/ShadowMap.h"
#include"PostProcessing/BlurFilter.h"
#include"PostProcessing/SSAO.h"
#include"JGLight.h"
#include"CommonData.h"
using namespace JGRC;
using namespace std;
using namespace DirectX;

Scene::Scene(class DxCore* core)
{
	// DX��ġ �� ȯ�� ���� �ҷ�����
	m_DxCore = core;
	m_SceneConfig = core->GetSettingDesc();

	// ���ҽ� �Ŵ��� �ʱ�ȭ
	m_ResourceManager = make_unique<ResourceManager>();
	m_ResourceManager->Init(m_DxCore);



	// �� �� ������ ���� ��� �ʱ�ȭ
	InitStaticMemberVar();



	// ������ ���ҽ��� ���͸��� ������ �ʱ�ȭ
	m_FrameResourceManager = make_unique<EngineFrameResourceManager>();
	m_MaterialCreater = make_unique<JGMaterialCreater>();



	// �������� �ʱ�ȭ �� ��Ʈ���� / �� ���̴�, pso ������ 
	m_SceneData = make_unique<SceneData>();
	m_CommonShaderRootSig = make_unique<CommonShaderRootSignature>();
	m_CommonShaderRootSig->RootSign(m_DxCore->Device());
	m_SceneShader = make_unique<Shader>();
	m_SceneShader->Init(global_scene_hlsl_path,
		{ EShaderType::Vertex, EShaderType::Pixel });
	m_ScenePSO = m_SceneShader->CompileAndConstrutPSO(EPSOMode::SCENE, m_CommonShaderRootSig.get(), {});



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
	// �� ������ ����
	m_SceneData->BuildSceneData(m_SceneConfig.Width, m_SceneConfig.Height);
	m_LightManager->BuildLight(m_CommonShaderRootSig.get());
	// ssao ����
	m_SSAO = make_unique<SSAO>();
	m_SSAO->BuildSSAO(m_SceneConfig.Width, m_SceneConfig.Height,
		m_DxCore->CommandList(), m_CommonShaderRootSig.get());
	m_Blur = make_unique<BlurFilter>(m_SceneConfig.Width, m_SceneConfig.Height);
	// ������Ʈ �и�
	for (auto& obj : m_ObjectMems)
	{
		obj->Build(m_DxCore->CommandList());
		EPSOMode Mode = obj->GetMaterial()->GetDesc()->Mode;
		EObjType Type = obj->GetType();

		m_ObjectArray[Type][Mode].push_back(obj.get());
	}
	for (auto& obj : m_InstanceObjects)
		obj->Build(m_DxCore->CommandList());


	// �޽� ����
	for (auto& mesh : m_MeshMems)
		mesh->CreateMesh(m_DxCore->Device(), m_DxCore->CommandList());


	// �ؽ��� �߰�
	for (auto& data : m_TextureDatas)
	{
		m_ResourceManager->AddTexture(m_DxCore->CommandList(), data.Path, data.Type);
	}
	// ���� ����Ʈ ���μ��� ����
	m_ResourceManager->BuildResourceHeap();
	m_MaterialCreater->BuildMaterial(m_CommonShaderRootSig.get());

	


	m_FrameResourceManager->BuildFrameResource(m_DxCore->Device(), 
		(UINT)max(1, m_PassDatas.size()), (UINT)max(1, m_ObjectMems.size() + 1),
		(UINT)max(1, m_MaterialCreater->Size()),
		(UINT)max(1, m_LightManager->Size()));
}
void Scene::OnReSize(UINT width, UINT height)
{
	m_SceneData->ReSize(width, height);
	m_SSAO->OnReSize(width, height);
	BoundingFrustum::CreateFromMatrix(m_Frustom, m_MainCamera->GetProj());
}
void Scene::Update(const GameTimer& gt)
{
	// �� ���� ������Ʈ
	m_SceneConfig = m_DxCore->GetSettingDesc();

	// ���� ī�޶� ������Ʈ
	m_MainCamera->UpdateViewMatrix();

	m_FrameResourceManager->FrameResourcePerFrame(m_DxCore->Fence());
	auto CurrFrameResource = m_FrameResourceManager->CurrentFrameResource();
	m_SSAO->Update(CurrFrameResource);
	// ������Ʈ ������Ʈ
	for (auto& obj : m_ObjectMems)
		obj->Update(gt, CurrFrameResource);
	for (auto& insObj : m_InstanceObjects)
		insObj->Update(gt, CurrFrameResource);

	// ���͸��� ������Ʈ
	for (auto& mat : m_MaterialCreater->GetArray())
		mat.second->Update(CurrFrameResource);

	// ����Ʈ �Ŵ��� 
	m_LightManager->Update(CurrFrameResource);

	// ���� �н� ������Ʈ
	MainPassUpdate(gt);

	// �������� ������Ʈ
	BoundingFrustum::CreateFromMatrix(m_Frustom, m_MainCamera->GetProj());


	for (auto& objarr : m_ObjectArray)
	{
		for (auto& obj : objarr.second[EPSOMode::DEFAULT])
		{
			ObjCulling(obj);
		}
	}
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
	for (auto& obj : m_InstanceObjects)
	{
		obj->CubeDraw(CurrFrameResource, CommandList);
	}


	// ���� �о� ���
	CommandList->SetGraphicsRootConstantBufferView((UINT)ECommonShaderSlot::cbPerPass,
		MainPassHandle());
	m_SceneData->SceneDataExtract(CurrFrameResource, CommandList);


	// SwapChain�� �׸��� ����
	m_DxCore->StartDraw();

	CommandList->SetPipelineState(m_ScenePSO);
	CommandList->IASetVertexBuffers(0, 0, nullptr);
	CommandList->IASetIndexBuffer(nullptr);
	CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CommandList->DrawInstanced(6, 1, 0, 0);
	m_Blur->Execute(m_DxCore->CurrentBackBuffer(), CommandList, D3D12_RESOURCE_STATE_RENDER_TARGET, 1);
	// ������
	m_DxCore->EndDraw();

	CurrFrameResource->Fence = m_DxCore->CurrentFence();
}
JGRCObject* Scene::CreateObject(EObjType Type)
{
	auto Obj = make_unique<JGRCObject>(++m_ObjIndex, Type);
	JGRCObject* result = Obj.get();

	m_ObjectMems.push_back(move(Obj));
	return result;
}
InstanceObject* Scene::CreateInstanceObject(JGMesh* Mesh, const std::string& meshname, JGMaterial* mat)
{
	if (mat->GetDesc()->Mode != EPSOMode::INSTANCE)
		return nullptr;
	auto InsObj = make_unique<InstanceObject>();
	InstanceObject* result = InsObj.get();
	m_InstanceObjects.push_back(move(InsObj));


	result->SetMaterial(mat);
	result->SetMesh(Mesh, meshname);
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
	JGMesh* SkyMesh = AddMesh();
	SkyMesh->AddBoxArg("Scene_SkyBox_Default_Mesh", 1.0f, 1.0f, 1.0f, 0);
	// ������Ʈ ����
	JGRCObject* obj = CreateObject();
	obj->SetMesh(SkyMesh, "Scene_SkyBox_Default_Mesh");
	obj->SetMaterial(SkyMat);
	obj->SetScale(5000.0f, 5000.0f, 5000.0f);
	return obj;
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
bool    Scene::ObjCulling(JGRCObject* obj)
{
	BoundingFrustum localFrustom;
	XMMATRIX view    = m_MainCamera->GetView();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);

	XMMATRIX World = XMLoadFloat4x4(&obj->GetWorld());
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(World), World);

	XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);
	m_Frustom.Transform(localFrustom, viewToLocal);


	if ((localFrustom.Contains(obj->GetCullingBox())
		!= DISJOINT))
	{
		obj->ThisIsNotCulling();
		return false;
	}
	else
	{
		obj->ThisIsCulling();
		return true;
	}
	

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
	CommonData(m_DxCore, this, m_ResourceManager.get());
	CubeMap(512, 512);
	ShadowMap(2048, 2048);
}