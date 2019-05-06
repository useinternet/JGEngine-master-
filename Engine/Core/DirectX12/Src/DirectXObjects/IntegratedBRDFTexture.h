#pragma once
#include"DirectXCommon/DirectXCommon.h"
#include"DirectXToolKit/Texture.h"
#include"DirectXToolKit/PSO.h"
#include"DirectXToolKit/Viewport.h"
#include"DirectXToolKit/ScissorRect.h"
#include"DirectXToolKit/RenderTarget.h"



namespace Dx12
{
	// �� �ؽ��Ĵ� �������ڸ��� �׸���..
	class IntegratedBRDFTexture
	{
	private:
		static const DXGI_FORMAT ms_Format = DXGI_FORMAT_R32G32_FLOAT;
	private:

		GraphicsPSO  m_PSO;
		RenderTarget m_RenderTarget;
		Viewport     m_Viewport;
		ScissorRect  m_ScissorRect;

		uint32_t m_Width, m_Height;
	public:
		static DXGI_FORMAT GetFormat() {
			return ms_Format;
		}
		const Texture& GetTexture() const;

	private:
		void Build();
	};
}