#pragma once
#include"DxCommon/DxCommon.h"

#include"Animation/JGAnimation.h"
#include"Debug/DebugBox.h"
#include"Debug/DebugScreen.h"
#include"DxCore/SceneConfig.h"

namespace JGRC
{
	class SceneData;
	class CommandListManager;
	class ScreenManager;
	class GpuCpuSynchronizer;
	class DxDevice;
	class JGRCObject;
	class JGMaterial;
	class JGStaticMesh;
	class JGSkeletalMesh;
	class LightManager;
	class JGLight;

	enum class ELightType;
	enum class ELightExercise;
	enum class EObjType;
	class RCORE_EXPORT Scene
	{
		typedef std::unordered_map<EObjType,
			    std::unordered_map<EPSOMode,
			    std::vector<JGRCObject*>>>
			    ObjectArray;
	private:
		std::string m_Name;
		DxDevice* m_DxDevice = nullptr;
		SceneConfig m_SceneConfig;

		// �޸� �� ������ ������
		ResourceManager*            m_ResourceManager;
		CommandListManager*         m_CmdListManager;
		ScreenManager*              m_ScreenManager;
		std::shared_ptr<SceneData>    m_SceneData;
		std::shared_ptr<class Shader> m_SceneShader;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_ScenePSO = nullptr;
		//  SSAO
		std::shared_ptr<class SSAO> m_SSAO;
		std::shared_ptr<class BlurFilter> m_Blur;
		// ���� ��� �н� 
		PassData* m_MainPass = nullptr;
		// ī�޶�
		std::vector<std::unique_ptr<Camera>> m_Cameras;
		Camera* m_MainCamera = nullptr;
		// ������Ʈ
		std::vector<JGRCObject*>      m_ObjectMems;
		ObjectArray m_ObjectArray;
		std::vector<std::unique_ptr<DebugBox>> m_DebugMems;
		// ���͸���
		std::vector<JGMaterial*>      m_MaterialMems;
		// �ִϸ��̼�
		std::vector<std::unique_ptr<JGAnimation>>     m_AnimationMems;
		std::unordered_map<std::string, JGAnimation*> m_Animations;
		// �ؽ��� ���
		std::vector<TexturePack> m_TextureDatas;
		// ��ī�� �ڽ� �� �� ��ü�� ���δ� ��
		DirectX::BoundingSphere m_SceneSphere;
		std::wstring m_SkyShaderPath = L"../Contents/Engine/Shaders/Sky.hlsl";
		std::unordered_map<std::wstring, JGRCObject*> m_SkyBox;
		JGRCObject* m_MainSkyBox = nullptr;
		// ����Ʈ 
		std::shared_ptr<LightManager> m_LightManager;
		//
	private:
		Scene(const Scene& rhs) = delete;
		Scene& operator=(const Scene& rhs) = delete;
	public:
		Scene(const std::string& name, const SceneConfig& config);
		void BuildScene();
		void OnReSize(UINT width, UINT height);
		void Update(const GameTimer& gt);
		void Draw();
		void SceneObjectDraw(ID3D12GraphicsCommandList* CommandList, FrameResource* CurrFrameResource, EObjRenderMode Mode, bool bDebug = false);
		void SceneStaticObjectDraw(ID3D12GraphicsCommandList* CommandList, FrameResource* CurrFrameResource, EObjRenderMode Mode);
		void SceneDynamicObjectDraw(ID3D12GraphicsCommandList* CommandList, FrameResource* CurrFrameResource, EObjRenderMode Mode);
	public:
		JGRCObject*     CreateObject(JGMaterial* mat, JGBaseMesh* mesh, const std::string& meshname, EObjType Type);
		JGRCObject*     CreateSkyBox(const std::wstring& texturepath);
		void            AddDebugBox(JGRCObject* obj, const DirectX::XMFLOAT3& color, float thickness = 0);
		JGMaterial*     AddMaterial(const MaterialDesc& Desc);
		JGStaticMesh*   AddStaticMesh();
		JGSkeletalMesh* AddSkeletalMesh();
		std::string     AddAnimation(const std::string& path);
		Camera*         AddCamera();
		PassData*       AddPassData();
		JGLight*        AddLight(ELightType type, ELightExercise extype);
		void            AddTexture(const std::wstring& TexturePath, ETextureType type = ETextureType::Default);
	public:
		Camera*      GetMainCamera() const               { return m_MainCamera; }
		void         SetMainCamera(Camera* cam)          { m_MainCamera = cam; }
		JGRCObject*  GetMainSkyBox() const; 
		void         SetMainSkyBox(JGRCObject* skybox);
		const SceneConfig&   GetConfig() const           { return m_SceneConfig; }
		ID3D12PipelineState* GetScenePSO() const         { return m_ScenePSO.Get(); }
		JGAnimation*  GetAnimation(const std::string& name);
		SceneData*    GetSceneData() const;
		LightManager* GetLightManager();
		const std::string& GetName() const { return m_Name; }
	public:
		const cbPassConstant& MainPassData()  const    { return m_MainPass->Data; }
		PassData*             GetMainPass()            { return m_MainPass; }
		D3D12_GPU_VIRTUAL_ADDRESS MainPassHandle();
		const DirectX::BoundingSphere& GetSceneSphere()      const { return m_SceneSphere; }
		std::vector<JGRCObject*>& GetArray(EObjType objType, EPSOMode mode);
	private:
		void MainPassUpdate(const GameTimer& gt);
	};
}