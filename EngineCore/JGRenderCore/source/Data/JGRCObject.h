#pragma once
#include"DxCommon/DxCommon.h"

namespace JGRC
{
	// ���߿� Static, Dynamic ������Ʈ�� ������..
	enum class EObjType
	{
		Static,
		Dynamic,
		Instance
	};
	enum class EObjRenderMode
	{
		Default,
		NonePSO,
		ViewNormal,
		Shadow
	};
	typedef DirectX::XMFLOAT3 Vec3;

	class RCORE_EXPORT JGRCObject
	{
		static UINT64 Count;
		static UINT SkinnedIndex;
	public:
		static UINT SkinnedCount();
	private:
		// �޽� & ���͸��� & �޽� �̸� �ֿ� ����
		class JGRCObject* m_Owner    = nullptr;
		class JGMaterial* m_Material = nullptr;
		class JGBaseMesh* m_Mesh     = nullptr;
		class JGSkeletalMesh* m_SkeletalMesh = nullptr;
		std::string       m_MeshName;

		// �ִϸ��̼� 
		class JGAnimation* m_Anim = nullptr;
		std::shared_ptr<class JGAnimationHelper> m_AnimHelper;

		// ť���
		std::shared_ptr<class CubeMap> m_CubeMap = nullptr;

		// Ÿ�� & ������Ʈ �̸� & �޸� �ε���
		EObjType    Type = EObjType::Static;
		std::string m_Name;
		UINT m_ObjCBIndex = 0;
		UINT m_SkinnedCBIndex = 0;

		// ���� ��� �� �ؽ��� ���
		DirectX::XMFLOAT4X4 m_World        = MathHelper::Identity4x4();
		DirectX::XMFLOAT4X4 m_TexTransform = MathHelper::Identity4x4();

		// ��ġ & ȸ�� & ������
		Vec3 m_Location = { 0.0f,0.0f,0.0f };
		Vec3 m_Rotation = { 0.0f,0.0f,0.0f };
		Vec3 m_Scale    = { 1.0f,1.0f,1.0f };


		// ������Ʈ ���� ( �ʱ�ȭ & ������ ���� & Ȱ��ȭ )
		bool m_bInit    = false;
		bool m_bVisible = true;
		bool m_bActive  = true;


		// ������ ���� �� PSO �� �ڿ� ����
		D3D12_PRIMITIVE_TOPOLOGY m_PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		CommonPSOPack m_PSOPack;
		int UpdateNotify = CPU_FRAMERESOURCE_NUM;

	public: // ���� & ������Ʈ & ť�� �� & ������
		JGRCObject(UINT Index, EObjType Type, const std::string& name = "JGRCObject");
		void Build(ID3D12GraphicsCommandList* CommandList);
		void Update(const GameTimer& gt, FrameResource* CurrentFrameResource);
		void CubeMapDraw(FrameResource* CurrentFrameResource, ID3D12GraphicsCommandList* CommandList);
		void Draw(class FrameResource* CurrentFrameResource, ID3D12GraphicsCommandList* CommandList, EObjRenderMode Mode = EObjRenderMode::Default);
	public: 
		void UpdateWorldMatrix();
		EObjType GetType()              { return Type; }
		UINT GetCBIndex() const         { return m_ObjCBIndex; }
		void ClearNotify()    { UpdateNotify = CPU_FRAMERESOURCE_NUM;}
		void UpdatePerFrame() { UpdateNotify--; }
		bool IsCanUpdate()    { return UpdateNotify > 0; }
		void UnVisible()      { m_bVisible = false; }
		void Visible()        { m_bVisible = true; }
		bool IsVisible()      { return m_bVisible; }
		void Active()         { m_bActive = true; }
		void DeActive()       { m_bActive = false; }
		bool IsActive()       { return m_bActive; }
	public:
		JGMaterial* GetMaterial() const { return m_Material; }
		JGBaseMesh* GetMesh() const { return m_Mesh; };
		const std::string& GetMeshName() const { return m_MeshName; }
		const std::string& GetObjName() const  { return m_Name; }
		void SetMesh(JGBaseMesh* mesh, const std::string& meshname);
		void SetMaterial(JGMaterial* material);
		void SetAnimation(const std::string& name);
		void AttachTo(JGRCObject* obj);
	public:
		Vec3 GetLocation() const { return m_Location; }
		Vec3 GetRotation() const { return m_Rotation; }
		Vec3 GetScale()    const { return m_Scale; }
		const DirectX::XMFLOAT4X4& GetWorld()        { return m_World; }
		const DirectX::XMFLOAT4X4& GetTexTransform() { return m_TexTransform; }
	public:
		void SetLocation(float x, float y, float z);
		void SetRotation(float pitch, float yaw, float roll);
		void SetScale(float x, float y, float z);
		void SetScale(float x);
	public:
		void OffsetLocation(float x, float y, float z);
		void OffsetRotation(float pitch, float yaw, float roll);
		void OffsetScale(float x, float y, float z);
		void OffsetScale(float x);
	};

}