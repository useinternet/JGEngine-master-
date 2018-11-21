#pragma once
#include"SRSRenderTarget.h"
#include"SRSScene.h"
#include"Light/DirectionLight.h"
#include"Light/PointLight.h"
#include"Light/SpotLight.h"
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
		uint m_MaxPointLightCount = 20;
		uint m_MaxSpotLightCount  = 10;
		int m_DirectionLightCount = 0;
		int m_PointLightCount     = 0;
		int m_SpotLightCount      = 0;
		int m_TempCount2 = 0;


		typedef std::vector<PointLight> PointLightArray;
		typedef std::vector<SpotLight>  SpotLightArray;
	private:
		HWND m_hWnd;
		class DirectX* m_Dx;
		class JGShaderArray*  m_Shader;
		// ��
		std::unique_ptr<SRSScene>        m_Scene;
		SRSRenderTarget* m_RenderTarget = nullptr;
		// ����Ʈ
		std::unique_ptr<DirectionLight> m_DirectionLight;
		PointLightArray m_PointLightArray;
		SpotLightArray  m_SpotLightArray;
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
		DirectionLight* GetDirectionLight() { return m_DirectionLight.get(); }
		DirectionLight* AddDirectionLight();
		PointLight* AddPointLight();
		SpotLight*  AddSpotLight();
		void        DeletePointLight(PointLight* light);
		void        DeleteSpotLight(SpotLight* light);
		void BindingCamera(Camera* cam);
	private:
		void InitHlsl(const DxWinConfig& config);
	};
}