#pragma once
#include"DxCommon/DxCommon.h"
#include"JGRCObject.h"
#include"JGMaterial.h"
#include"JGMesh.h"
#include"LightManager.h"
#include"DxCore/DxSetting.h"
/*
����Ʈ ����
��� ���� �� ���� ���� ����
�׸��� ������Ʈ Static, Dynamic �и�


--- �ؾ��Ұ� ( ���� �ɸ��� �𸥴�. ) 
1. ���ҽ��Ŵ��� ������
2. Draw�Լ��� CommandList �Ű������� �޾Ƽ� Draw
3. Update, Draw�Լ� FrmaeResource* �Ű����� �޾Ƽ� �Ұ�
4. ������ ��ü������ �ݻ�ǰԲ� ����
5. ���� ������ ���� �ϸ鼭 ���̴� ����
6. �Թ�ü �� Ư�� ������� ���Ͽ� �� ó��( �����ڸ� �� �ƴϸ� ���콺 �� ) <- �׸��� ��� �Թ�ü�� 
�����Ҽ��ְԲ� ���� ( unitrayDir �� ���� posW �� posH ���� center���� ���ϴ� ���� �� �ʺ��� )
7. �׸��� ��ó�� ( ���� �׸��� ���� ���� �� �ǽð� ���� �����Ҽ��ְ� ����)
8. �ν��Ͻ� ������Ʈ ����
9. ������Ʈ ���� ���� �Ҽ��ְ� �����丵( ���� ������Ʈ Ÿ�Ժ� ������ PSO �� �����з� �۾� �����丵)
10. ��Ʈ���� �̿��� �������� �ø� �� ��ŷ �ý��� ����
11. �� �� �α� �� �������ϸ� �� FBX �ҷ����� (JGMesh ����) 
12. �ڵ� �ٵ�� �ִϸ��̼�
*/

namespace JGRC
{
	class RCORE_EXPORT Scene
	{
	private:
		class DxCore* m_DxCore = nullptr;
		DxSetting m_SceneConfig;

		// �޸� �� ������ ������
		std::unique_ptr<ResourceManager> m_ResourceManager;
		std::unique_ptr<EngineFrameResourceManager> m_FrameResourceManager;
		// ���� ��� �н� 
		std::vector<std::unique_ptr<PassData>> m_PassDatas;
		UINT m_PassCBIndex = -1;
		PassData* m_MainPass = nullptr;
		// ī�޶�
		std::vector<std::unique_ptr<Camera>> m_Cameras;
		Camera* m_MainCamera = nullptr;
		// ������Ʈ
		std::vector<std::unique_ptr<JGRCObject>>  m_ObjectMems;
		std::unordered_map<EPSOMode, std::vector<JGRCObject*>> m_Objects;
		UINT m_ObjIndex = -1;

		// ���͸���
		std::unique_ptr<JGMaterialCreater>           m_MaterialCreater;
		std::unordered_map<std::string, JGMaterial*> m_Materials;

		// �޽�
		std::vector<std::unique_ptr<JGMesh>>         m_MeshMems;
		std::unordered_map<std::string, JGMesh*>     m_Meshs;

		// �ؽ��� ���
		std::vector<TexturePack> m_TextureDatas;
		std::vector<CD3DX12_GPU_DESCRIPTOR_HANDLE>   m_SkyCubeTextureHandles;
		CD3DX12_GPU_DESCRIPTOR_HANDLE*               m_MainSkyBoxHandle = nullptr;

		// ��ī�� �ڽ� �� �� ��ü�� ���δ� ��
		JGRCObject* m_MainSkyBox = nullptr;
		std::wstring m_TextureName;
		DirectX::BoundingSphere m_SceneSphere;

		// ����Ʈ 
		std::unique_ptr<LightManager> m_LightManager;
		std::unique_ptr<JGRCObject> m_DebugObj;
		//��� �� �ؽ��� 
		// �ӽ�
		std::shared_ptr<class SSAO>   m_SSAO;
	private:
		Scene(const Scene& rhs) = delete;
		Scene& operator=(const Scene& rhs) = delete;
	public:
		Scene(class DxCore* core);
		void BuildScene();
		void OnReSize(UINT width, UINT height);
		void Update(const GameTimer& gt);
		void Draw();
	public:
		JGRCObject* CreateObject();
		JGMaterial* AddMaterial(const MaterialDesc& Desc);
		JGMesh*     AddMesh();
		Camera*     AddCamera();
		PassData*   AddPassData();
		JGLight*    AddLight(ELightType type);
		void        AddTexture(const std::wstring& TexturePath, ETextureType type = ETextureType::Default);
		UINT        GetSkyBoxShaderIndex();
	public:
		Camera*   GetMainCamera() const      { return m_MainCamera; }
		void      SetMainCamera(Camera* cam) { m_MainCamera = cam; }
		const DxSetting& GetSetting() const         { return m_SceneConfig; }
	public:
		const cbPassConstant& MainPassData()  const    { return m_MainPass->Data; }
		JGRCObject*           GetMainSkyBox() const    { return m_MainSkyBox; }
		PassData*             GetMainPass()            { return m_MainPass; }
		D3D12_GPU_VIRTUAL_ADDRESS MainPassHandle();
		const DirectX::BoundingSphere& GetSceneSphere()      const { return m_SceneSphere; }
		D3D12_GPU_DESCRIPTOR_HANDLE    GetMainSkyBoxHandle() const { return *m_MainSkyBoxHandle; }
		std::vector<std::unique_ptr<JGRCObject>>& GetArray()       { return m_ObjectMems; }
		std::vector<JGRCObject*>& GetArray(EPSOMode mode)         { return m_Objects[mode]; }
	private:
		void CreateDebugObj();
		void MainPassUpdate(const GameTimer& gt);
		void InitStaticMemberVar();
	};
}