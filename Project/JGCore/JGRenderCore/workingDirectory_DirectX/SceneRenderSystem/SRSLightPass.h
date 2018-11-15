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
		std::unique_ptr<SRSRenderTarget> m_RenderTarget;
	public:
		SRSLightPass();
		virtual ~SRSLightPass();

		void Init(HWND hWnd,const uint width, const uint height);
		// ���߿� ����ƮŬ������ �����´�.
		void Render(const jgVec3& CameraPos);

	private:
		void InitHlsl();
	};
}