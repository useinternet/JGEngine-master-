#pragma once
#include"SRSRenderTarget.h"
#include"SRSScene.h"



namespace JGRC
{
	class CORE_EXPORT SRSLightPass
	{
	private:
		HWND m_hWnd;
		class DirectX* m_Dx;
		class JGShaderArray*  m_Shader;
		std::unique_ptr<SRSScene>        m_Scene;
		SRSRenderTarget* m_RenderTarget = nullptr;
	public:
		SRSLightPass();
		virtual ~SRSLightPass();
		void Init(const DxWinConfig& config, SRSRenderTarget* SRSRT);
		// ���߿� ����ƮŬ������ �����´�.
		void Render();
	private:
		void InitHlsl(const DxWinConfig& config);
	};
}