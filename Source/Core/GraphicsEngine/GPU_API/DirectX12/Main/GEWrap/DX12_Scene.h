#pragma once
#include "Scene.h"
#include "DirectXApi.h"




namespace DX12
{
	REGISTER_GRAPHICS_INTERFACE_SCENE(DX12_Scene)


	class DX12_Scene : public GE::Scene
	{
	public:
		DX12_Scene();
		virtual void BindCamera(GE::SceneCamera* cam) override;
		virtual void UnBindCamera() override;



		virtual void PushObject(GE::SceneObject* sceneObject) override;
		virtual void PushObjects(const std::vector<GE::SceneObject*>& sceneObject) override;
		virtual void FlushObjects() override;


		virtual GE::SceneCamera* GetBindedCamera() const override;
		virtual std::vector<GE::SceneObject*> GetPushedObjects() const override;

		virtual GE::Texture* GetSceneTexture() const override;

		virtual ~DX12_Scene() override;
	private:
		void TextureSetting() const;
	private:
		mutable std::mutex m_TMutex;
		GE::SceneCamera* m_Cam = nullptr;
		std::vector<GE::SceneObject*> m_SceneObjs;
		std::unique_ptr<GE::Texture> m_SceneTexture = nullptr;
	};
}