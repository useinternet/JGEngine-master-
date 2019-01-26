#pragma once
#include"DxCommon/DxCommon.h"
#include"Shader/CommonShaderRootSignature.h"
#include"Shader/Shader.h"

// �ν��Ͻ� ���� ��ũ�� ������ ���� ���� ��ΰ��.
namespace JGRC
{
	enum class ETextureSlot
	{
		Diffuse = 0,
		Normal  = 1,
		Custom0,
		Custom1,
		Custom2,
		Custom3,
		Custom4,
		Custom5
	};
	/*
	Exp : ���͸��� ��� 
	* �̸�
	* PSO ���
	* ���̴� ���
	* �ݻ�� ����
	* ������ ���� */
	struct MaterialDesc
	{
		std::string  Name;
		EPSOMode  Mode = EPSOMode::DEFAULT;
		std::wstring ShaderPath;
		bool bCubMapDynamic = false;
		bool bCubeMapStatic = false;
	};
	//
	class RCORE_EXPORT JGMaterial
	{
	private:
		// ���͸��� �� �ε��� �� PSO
		UINT m_MatCBIndex  = 0;

		// ���͸��� ������ (���, ���� ��)
		std::unique_ptr<MaterialData> m_Data;
		std::unique_ptr<MaterialDesc> m_Desc;
		// ������ �ؽ��� ������Ʈ�� ���� Que
		std::queue<std::pair<ETextureSlot, std::wstring>> m_TextureDataQue;
		std::unordered_map<ETextureSlot, std::wstring>    m_TexturePaths;
		// ������ �˸���
		int UpdateNotify = CPU_FRAMERESOURCE_NUM;
	private:
		JGMaterial(const JGMaterial& material) = delete;
		JGMaterial& operator=(const JGMaterial& material) = delete;
	public:
		JGMaterial(UINT Index, const MaterialDesc& desc);
		~JGMaterial() {}
	public:
		void Update(class FrameResource* CurrentFrameResource);
		std::vector<ShaderMacroPack> GetMacroPack();
	public:  // ���͸��� ������ Set ������.
		void SetTexture(ETextureSlot slot,const std::wstring& TexturePath);
		void SetMaterialData(const MaterialData& Data);
		void SetDiffuseAlbedo(float r, float g, float b, float a);
		void SetFresnelR0(float x, float y, float z);
		void SetRoughness(float x);
		void SetRefractive(float x);
		void SetReflectivity(float x);
		void SetMatTransform(const DirectX::XMFLOAT4X4& mat);
	public:
		const std::wstring& GetTexturePath(ETextureSlot slot) { return m_TexturePaths[slot]; }
	public:  // ���͸��� Get������
		MaterialDesc*       GetDesc() const { return m_Desc.get(); }
		const MaterialData& GetData() const { return *m_Data; }
		UINT CBIndex() const { return m_MatCBIndex; }
	private: // ������ �˸� �Լ���
		void ClearNotify() { UpdateNotify = CPU_FRAMERESOURCE_NUM; }
		void UpdatePerFrame() { UpdateNotify--; }
		bool IsCanUpdate()    { return UpdateNotify > 0; }
	};
	//
	//
	//
	class RCORE_EXPORT JGMaterialCreater
	{
		typedef std::unordered_map<std::string,  std::unique_ptr<JGMaterial>> MaterialArray;
	private:
		MaterialArray m_Materials;
		int m_MatCBIndex = -1;
	private:
		JGMaterialCreater(const JGMaterialCreater& copy) = delete;
		JGMaterialCreater& operator=(const JGMaterialCreater& rhs) = delete;
	public:
		JGMaterialCreater()  = default;
		~JGMaterialCreater() = default;
	public: // ���͸��� ���� �� ��������
		JGMaterial*    CreateMaterial(const MaterialDesc& Desc);
		JGMaterial*    GetMaterial(const std::string& name);
		MaterialArray& GetArray() { return m_Materials; }
	public:
		UINT Size() const { return (UINT)m_Materials.size(); }
	};
}