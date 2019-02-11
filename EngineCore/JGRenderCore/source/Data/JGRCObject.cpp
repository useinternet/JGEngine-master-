#include"JGRCObject.h"
#include"Mesh/JGStaticMesh.h"
#include"Mesh/JGSkeletalMesh.h"
#include"JGMaterial.h"
#include"EngineFrameResource.h"
#include"Shader/CommonShaderRootSignature.h"
#include"Scene.h"
#include"PostProcessing/CubeMap.h"
#include"CommonData.h"
#include"Animation/JGAnimation.h"
using namespace JGRC;
using namespace std;
using namespace DirectX;
UINT64 JGRCObject::Count        = 0;
UINT JGRCObject::SkinnedIndex = 0;
UINT JGRCObject::SkinnedCount() { return SkinnedIndex; }
JGRCObject::JGRCObject(UINT Index, EObjType Type, const string& name)
{
	m_Name += name + to_string(Index);
	this->Type = Type;
	m_ObjCBIndex = Index;
	UpdateWorldMatrix();
	Count++;
}
void JGRCObject::Build(ID3D12GraphicsCommandList* CommandList, CommonShaderRootSignature* RoogSig)
{	
	m_bInit = true;

	//
	MaterialDesc* Desc = m_Material->GetDesc();
	/* ť�� �� Ȱ��ȭ�� ť��� ����
	*/
	if((Desc->bCubeMapStatic || Desc->bCubMapDynamic))
	{
		m_CubeMap = make_unique<CubeMap>();
		m_CubeMap->BuildCubeMap(m_Name, CommandList);
		m_CubeMap->BuildCamera(m_Location.x, m_Location.y, m_Location.z);
	}

	// PipeLineState ( �޽� & ���͸��� & ������Ʈ ��Ȳ�� ���� ���̴��� ��ũ�η� ���� )
	EShaderFlag flag = m_Material->GetFlag();
	if (m_Mesh->Type() == EMeshType::Skeletal && m_Anim)
	{
		Type = EObjType::Dynamic;
		flag = flag | Shader_Flag_Skinned;

		m_PSOPack.CompilePSO(
			m_Material->GetDesc()->ShaderPath,
			m_Material->GetDesc()->Mode, 
			CommonData::_Scene()->GetSkinnedRootSig(),
			flag);

		m_SkinnedCBIndex = (UINT)SkinnedIndex++;
		m_AnimHelper = make_unique<JGAnimationHelper>();
		m_SkeletalMesh = dynamic_cast<JGSkeletalMesh*>(m_Mesh);
		m_AnimHelper->BuildAnimationData(m_Anim, m_SkeletalMesh->GetBoneHierarchy(m_MeshName), (UINT)m_SkeletalMesh->GetBoneData(m_MeshName).size(), m_MeshName);
	}
	else
	{
		m_PSOPack.CompilePSO(
			m_Material->GetDesc()->ShaderPath,
			m_Material->GetDesc()->Mode, 
			CommonData::_Scene()->GetRootSig(),
			flag);
	}
}
void JGRCObject::Update(const GameTimer& gt,FrameResource* CurrentFrameResource)
{
	if (!m_bActive)
		return;

	// ť��� & ������Ʈ ������Ʈ
	auto ObjCB = CurrentFrameResource->ObjectCB.get();
	if (m_CubeMap)
	{
		m_CubeMap->Update(gt, CurrentFrameResource);
	}
	if (IsCanUpdate())
	{
		cbObjectConstant objConstants;
		if (m_CubeMap) 
		{
			m_CubeMap->BuildCamera(m_Location.x, m_Location.y, m_Location.z);
			objConstants.CubeMapIndex = m_CubeMap->GetCubeMapIndex();
		}
		else
			objConstants.CubeMapIndex = CommonData::_ResourceManager()->GetCubeTextureShaderIndex(
				CommonData::_Scene()->GetMainSkyBox()->GetMaterial()->GetTexturePath(ETextureSlot::Diffuse));


		UpdateWorldMatrix();

		XMMATRIX World = XMLoadFloat4x4(&m_World);
		XMMATRIX TexTransform = XMLoadFloat4x4(&m_TexTransform);


		XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(World));
		XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(TexTransform));
		objConstants.MaterialIndex = m_Material->CBIndex();
		ObjCB->CopyData(m_ObjCBIndex, objConstants);

		UpdatePerFrame();
	}

	// �ִϸ��̼� ������Ʈ

	if (m_Anim && m_Mesh->Type() == EMeshType::Skeletal)
	{
		m_AnimHelper->UpdateAnimatoin(gt, m_Anim, m_SkeletalMesh->GetBoneHierarchy(m_MeshName),
			(UINT)m_SkeletalMesh->GetBoneData(m_MeshName).size());
		if (m_AnimHelper->IsPlay)
			CurrentFrameResource->SkinnedCB->CopyData(m_SkinnedCBIndex, m_AnimHelper->Get());
	}
}
void JGRCObject::CubeMapDraw(FrameResource* CurrentFrameResource, ID3D12GraphicsCommandList* CommandList)
{
	if (!m_bVisible)
		return;
	MaterialDesc* Desc = m_Material->GetDesc();

	// ť����� �����ϰ�  ���� ť��ʵ� �ƴϰ� ������Ʈ�� �ٳ����ٸ� �׸��� �ʴ´�.
	if (m_CubeMap && !Desc->bCubMapDynamic && !IsCanUpdate())
		return;
	if (m_CubeMap)
	{
		m_CubeMap->Excute(CurrentFrameResource, CommandList);
	}
}
void JGRCObject::Draw(FrameResource* CurrentFrameResource, ID3D12GraphicsCommandList* CommandList, EObjRenderMode Mode)
{
	if (!m_bVisible || Type == EObjType::Instance)
	{
		return;
	}
	// �� ����������( �⺻ & �׸��� & viewNormal ) ������ �ٸ��� PSO�� ���� �� �������Ѵ�.
	auto ObjCB = CurrentFrameResource->ObjectCB->Resource();
	auto MeshData = m_Mesh->Data();
	UINT ObjCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(cbObjectConstant));

	switch (Mode)
	{
	case EObjRenderMode::Default:
		CommandList->SetPipelineState(m_PSOPack.CustomPSO);
		break;
	case EObjRenderMode::Shadow:
		CommandList->SetPipelineState(m_PSOPack.ShadowPSO);
		break;
	case EObjRenderMode::ViewNormal:
		CommandList->SetPipelineState(m_PSOPack.ViewNormalPSO);
		break;
	}
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = ObjCB->GetGPUVirtualAddress();
	objCBAddress += (m_ObjCBIndex * ObjCBByteSize);
	CommandList->SetGraphicsRootConstantBufferView((UINT)ECommonShaderSlot::cbPerObject, objCBAddress);

	// �޽ð� ���̷��� �޽� �̰� �ִϸ��̼��� Ȱ��ȭ �Ǿ��ִ°�� ���̴��� �� Ʈ������ ������ ����
	if (m_Mesh->Type() == EMeshType::Skeletal && m_Anim && m_AnimHelper->IsPlay)
	{
		UINT skbtSize = d3dUtil::CalcConstantBufferByteSize(sizeof(SkinnedData));
		D3D12_GPU_VIRTUAL_ADDRESS Address = CurrentFrameResource->SkinnedCB->Resource()->GetGPUVirtualAddress();
		Address += (m_SkinnedCBIndex * skbtSize);

		CommandList->SetGraphicsRootConstantBufferView((UINT)ECommonShaderSlot::cbPerSkinned, Address);
	}
	m_Mesh->ArgDraw(m_MeshName, CommandList, CurrentFrameResource);
}
void JGRCObject::UpdateWorldMatrix()
{

	XMMATRIX Translation = XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);
	XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_Rotation.x),
		XMConvertToRadians(m_Rotation.y),
		XMConvertToRadians(m_Rotation.z));
	XMMATRIX Scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	XMMATRIX World;
	(m_Owner) ? 
		World = Scale * Rotation * Translation * XMLoadFloat4x4(&m_Owner->GetWorld()) :
		World = Scale * Rotation * Translation;
	
	XMStoreFloat4x4(&m_World, World);
}
void JGRCObject::SetMesh(JGBaseMesh* mesh, const std::string& meshname)
{
	m_Mesh = mesh;
	m_MeshName = meshname;
	ClearNotify();
}
void JGRCObject::SetMaterial(JGMaterial* material)
{
	m_Material = material;
	ClearNotify();
}
void JGRCObject::SetAnimation(const string& name)
{
	m_Anim = CommonData::_Scene()->GetAnimation(name);
}
void JGRCObject::AttachTo(JGRCObject* obj)
{
	if (this == obj)
		nullptr;
	obj->m_Owner = this;

}
void JGRCObject::SetLocation(float x, float y, float z)
{
	if (m_bInit && Type == EObjType::Static)
		return;
	m_Location = { x,y,z };
	ClearNotify();
}
void JGRCObject::SetRotation(float pitch, float yaw, float roll)
{
	if (m_bInit && Type == EObjType::Static)
		return;
	m_Rotation = { pitch, yaw, roll };
	ClearNotify();
}
void JGRCObject::SetScale(float x, float y, float z)
{
	if (m_bInit && Type == EObjType::Static)
		return;
	m_Scale = { x,y,z };
	ClearNotify();
}
void JGRCObject::SetScale(float x)
{
	if (m_bInit && Type == EObjType::Static)
		return;
	m_Scale = { x,x,x };
	ClearNotify();
}

void JGRCObject::OffsetLocation(float x, float y, float z)
{
	if (m_bInit && Type == EObjType::Static)
		return;
	m_Location = { m_Location.x + x, m_Location.y + y, m_Location.z + z };
	ClearNotify();
}
void JGRCObject::OffsetRotation(float pitch, float yaw, float roll)
{
	if (m_bInit && Type == EObjType::Static)
		return;
	m_Rotation = { m_Rotation.x + pitch, m_Rotation.y + yaw, m_Rotation.z + roll };
	ClearNotify();
}
void JGRCObject::OffsetScale(float x, float y, float z)
{
	if (m_bInit && Type == EObjType::Static)
		return;
	m_Scale = { m_Scale.x + x, m_Scale.y + y, m_Scale.z + z };
	ClearNotify();
}
void JGRCObject::OffsetScale(float x)
{
	if (m_bInit && Type == EObjType::Static)
		return;
	m_Scale = { m_Scale.x + x, m_Scale.y + x, m_Scale.z + x };
	ClearNotify();
}