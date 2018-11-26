#pragma once
#include"SRSRenderTarget.h"
#include"SRSScene.h"
#include"LightPassLightManager.h"
#include"Camera/Camera.h"

// ���̴� ���� �������� �ƴҶ� ������
// ����Ʈ ���� ����
// dx�����쿡 �������� �ø� ����
// ť��� ����
// �ݻ�� ����
// �׸��� ����
namespace JGRC
{
	class CORE_EXPORT SRSLightPass
	{
	private:
		HWND m_hWnd;
		class DirectX* m_Dx;
		class JGShaderArray*  m_Shader;
		// ��
		std::unique_ptr<SRSScene>        m_Scene;
		SRSRenderTarget* m_RenderTarget = nullptr;
		//
		std::unique_ptr<LightPassLightManager> m_LightManager;
		// ī�޶�
		Camera* m_Camera = nullptr;
	private:
		SRSLightPass(const SRSLightPass& copy) = delete;
		SRSLightPass(SRSLightPass&& srp) = delete;
		SRSLightPass& operator=(const SRSLightPass& copy) = delete;
		SRSLightPass& operator=(SRSLightPass&& srp) = delete;
	public:
		SRSLightPass();
		virtual ~SRSLightPass();
		void Init(const DxWinConfig& config, SRSRenderTarget* SRSRT);
		// ���߿� ����ƮŬ������ �����´�.
		void Render();
	public:
		DirectionLight* GetDirectionLight();
		DirectionLight* AddDirectionLight();
		PointLight* AddPointLight();
		SpotLight*  AddSpotLight();
		void        DeletePointLight(PointLight* light);
		void        DeleteSpotLight(SpotLight* light);
		void BindingCamera(Camera* cam);
		LightPassLightManager* GetLightManager() { return m_LightManager.get(); }
		Camera* GetCamera()                      { return m_Camera; }
	private:
		virtual bool OutputHlsl();
		// ������ �̴°ſ� ������ �ҷ����°� �и�
		void InitHlsl(const DxWinConfig& config);
	};
}