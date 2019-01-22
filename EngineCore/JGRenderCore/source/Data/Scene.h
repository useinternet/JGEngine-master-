#pragma once
#include"DxCommon/DxCommon.h"
#include"LightManager.h"
#include"UserInterface/JGRCInterfaceClass.h"
#include"InstanceObject.h"
#include"JGMaterial.h"
#include"JGMesh.h"
#include"DxCore/DxSetting.h"
/*
--- �ؾ��Ұ� ( ���� �ɸ��� �𸥴�. ) 
10. ��Ʈ���� �̿��� �������� �ø� �� ��ŷ �ý��� ����
11. �� �� �α� �� �������ϸ� �� FBX �ҷ����� (JGMesh ����) 
12. �ڵ� �ٵ�� �ִϸ��̼�( ���� ��ü �ݻ�)
*/
namespace JGRC
{
	class RCORE_EXPORT Scene
	{
		typedef std::unordered_map<EObjType,
			    std::unordered_map<EPSOMode,
			    std::vector<JGRCObject*>>>
			    ObjectArray;


	private:
		class DxCore* m_DxCore = nullptr;
		DxSetting m_SceneConfig;

		// �޸� �� ������ ������
		std::unique_ptr<ResourceManager>            m_ResourceManager;
		std::unique_ptr<EngineFrameResourceManager> m_FrameResourceManager;
		std::shared_ptr<class SceneData>            m_SceneData;
		std::shared_ptr<class CommonShaderRootSignature> m_CommonShaderRootSig;
		std::shared_ptr<class Shader> m_SceneShader;
		ID3D12PipelineState* m_ScenePSO = nullptr;
		//  SSAO
		std::shared_ptr<class SSAO> m_SSAO;
		std::shared_ptr<class BlurFilter> m_Blur;
		// ���� ��� �н� 
		std::vector<std::unique_ptr<PassData>> m_PassDatas;
		UINT      m_PassCBIndex = -1;
		PassData* m_MainPass = nullptr;

		// ī�޶�
		std::vector<std::unique_ptr<Camera>> m_Cameras;
		Camera* m_MainCamera = nullptr;

		// ������Ʈ
		std::vector<std::unique_ptr<JGRCObject>>      m_ObjectMems;
		std::vector<std::unique_ptr<InstanceObject>>  m_InstanceObjects;
		ObjectArray m_ObjectArray;
		UINT m_ObjIndex = -1;

		// ���͸���
		std::unique_ptr<JGMaterialCreater>           m_MaterialCreater;
		std::unordered_map<std::string, JGMaterial*> m_Materials;

		// �޽�
		std::vector<std::unique_ptr<JGMesh>>         m_MeshMems;
		std::unordered_map<std::string, JGMesh*>     m_Meshs;

		// �ؽ��� ���
		std::vector<TexturePack> m_TextureDatas;

		// ��ī�� �ڽ� �� �� ��ü�� ���δ� ��
		DirectX::BoundingSphere m_SceneSphere;
		std::wstring m_SkyShaderPath = L"../Contents/Engine/Shaders/Sky.hlsl";
		std::unordered_map<std::wstring, JGRCObject*> m_SkyBox;
		JGRCObject* m_MainSkyBox = nullptr;
		// ��������
		DirectX::BoundingFrustum m_Frustom;
		// ����Ʈ 
		std::unique_ptr<LightManager> m_LightManager;
		//
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
		JGRCObject* CreateObject(JGMaterial* mat, JGMesh* mesh, const std::string& meshname, EObjType Type = EObjType::Static);
		InstanceObject* CreateInstanceObject(JGMesh* Mesh,const std::string& meshname, JGMaterial* mat);
		JGRCObject* CreateSkyBox(const std::wstring& texturepath);
		JGMaterial* AddMaterial(const MaterialDesc& Desc);
		JGMesh*     AddMesh();
		Camera*     AddCamera();
		PassData*   AddPassData();
		JGLight*    AddLight(ELightType type, ELightExercise extype = ELightExercise::Static);
		void        AddTexture(const std::wstring& TexturePath, ETextureType type = ETextureType::Default);
		void        SettingDefaultSceneBuffer(ID3D12GraphicsCommandList* CommandList, FrameResource* CurrFrameResource);
		bool        ObjCulling(JGRCObject* obj);
	public:
		Camera*   GetMainCamera() const      { return m_MainCamera; }
		void      SetMainCamera(Camera* cam) { m_MainCamera = cam; }
		JGRCObject* GetMainSkyBox() const    { return m_MainSkyBox; }
		void      SetMainSkyBox(JGRCObject* skybox)  { 
			if (skybox->GetMaterial()->GetDesc()->Mode == EPSOMode::SKYBOX)
				m_MainSkyBox = skybox;	}
		const DxSetting& GetSetting() const         { return m_SceneConfig; }
		ID3D12PipelineState* GetScenePSO() const   { return m_ScenePSO; }
	public:
		const cbPassConstant& MainPassData()  const    { return m_MainPass->Data; }
		PassData*             GetMainPass()            { return m_MainPass; }
		D3D12_GPU_VIRTUAL_ADDRESS MainPassHandle();
		const DirectX::BoundingSphere& GetSceneSphere()      const { return m_SceneSphere; }
		std::vector<JGRCObject*>& GetArray(EObjType objType, EPSOMode mode);
		std::vector<std::unique_ptr<InstanceObject>>& GetInstanceArray() { return m_InstanceObjects; }
	private:
		void MainPassUpdate(const GameTimer& gt);
		void InitStaticMemberVar();
	};
}