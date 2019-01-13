#include"JGMaterial.h"
#include"DxCore/DxCore.h"
#include"EngineFrameResource.h"
#include"CommonData.h"
using namespace JGRC;
using namespace std;
using namespace DirectX;

JGMaterial::JGMaterial(UINT Index, const MaterialDesc& desc)
{
	m_Data   = make_unique<MaterialData>();
	m_Desc   = make_unique<MaterialDesc>();
	*m_Desc = desc;
	m_MatCBIndex = Index;
}
void JGMaterial::Update(class FrameResource* CurrentFrameResource)
{
	auto MaterialCB = CurrentFrameResource->MaterialCB.get();
	if (IsCanUpdate())
	{
		// �ؽ��� ������Ʈ(������ ������Ʈ)
		while (!m_TextureDataQue.empty())
		{
			auto Data = m_TextureDataQue.front();
			m_TextureDataQue.pop();
			UINT idx = CommonData::_ResourceManager()->GetTextureIndex(Data.second);
			if (idx < 0)
				continue;
			m_Data->TextureIndex[(int)Data.first] = idx;
		}
		// ���͸��� ������ GPU�� ������ ���� ���ε���ۿ� ����
		XMMATRIX matTransform = XMLoadFloat4x4(&m_Data->MatTransform);
		MaterialData Data = *m_Data;
		XMStoreFloat4x4(&Data.MatTransform, XMMatrixTranspose(matTransform));
		UpdatePerFrame();
		MaterialCB->CopyData(m_MatCBIndex, Data);
	}
}
void JGMaterial::SetTexture(ETextureSlot slot,const std::wstring& TexturePath)
{
	m_TextureDataQue.push(pair<ETextureSlot, wstring>(slot, TexturePath));
	ClearNotify();
}
void JGMaterial::SetMaterialData(const MaterialData& Data)
{
	*m_Data = Data;
	ClearNotify();
}
void JGMaterial::SetDiffuseAlbedo(float r, float g, float b, float a)
{
	m_Data->DiffuseAlbedo = { r,g,b,a };
	ClearNotify();
}
void JGMaterial::SetFresnelR0(float x, float y, float z)
{
	m_Data->FresnelR0 = { x,y,z };
	ClearNotify();
}
void JGMaterial::SetRoughness(float x)
{
	m_Data->Roughness = x;
	ClearNotify();
}
void JGMaterial::SetRefractive(float x)
{
	m_Data->Refractive = x;
	ClearNotify();
}
void JGMaterial::SetMatTransform(const DirectX::XMFLOAT4X4& mat)
{
	m_Data->MatTransform = mat;
	ClearNotify();
}
void JGMaterial::SetPSO(ID3D12PipelineState* PSO)
{
	m_PSO = PSO;
}
vector<ShaderMacroPack> JGMaterial::CreateMacroPack()
{
	vector<ShaderMacroPack> v;
	if(m_Desc->bReflection)
		v.push_back(ShaderMacroPack{ SHADER_MACRO_DEFINE_REFLECTION, "1" });
	if (m_Desc->bRefraction)
		v.push_back(ShaderMacroPack{ SHADER_MACRO_DEFINE_REFRACTION, "1" });

	return move(v);
}
void JGMaterialCreater::BuildMaterial()
{
	m_RootSignature = make_unique<CommonShaderRootSignature>();
	// ���̴� ������ �� ���͸��� PSO ���
	m_RootSignature->RootSign(CommonData::_Device());
	for (auto& mat : m_Materials)
	{
		MaterialDesc* desc = mat.second->GetDesc();
		vector<ShaderMacroPack> macro = mat.second->CreateMacroPack();

		mat.second->SetPSO(m_Shaders[desc->ShaderPath]->CompileAndConstrutPSO(
			desc->Mode,
			m_RootSignature.get(),
			macro));
	}
}
JGMaterial* JGMaterialCreater::CreateMaterial(const MaterialDesc& Desc)
{
	// ���͸����� �̹� �ִٸ� �о�
	if (m_Materials.end() != m_Materials.find(Desc.Name))
		return nullptr;
	// ó�� ��� �ϴ� ���̴���� ���̴� �迭�� ����
	if (m_Shaders.end() == m_Shaders.find(Desc.ShaderPath))
	{
		auto shader = make_unique<Shader>();
		shader->Init(
			Desc.ShaderPath,
			{ EShaderType::Vertex, EShaderType::Pixel });
		m_Shaders[Desc.ShaderPath] = move(shader);
	}
	// ���͸��� ����
	auto Mat = make_unique<JGMaterial>(++m_MatCBIndex, Desc);
	JGMaterial* result = Mat.get();
	m_Materials[Desc.Name] = move(Mat);

	return result;
}
JGMaterial* JGMaterialCreater::GetMaterial(const string& name)
{
	if (m_Materials.end() == m_Materials.find(name))
		return nullptr;
	return m_Materials[name].get();
}
ID3D12RootSignature* JGMaterialCreater::GetRootSignature()
{
	return m_RootSignature->GetRootSignature();
}
