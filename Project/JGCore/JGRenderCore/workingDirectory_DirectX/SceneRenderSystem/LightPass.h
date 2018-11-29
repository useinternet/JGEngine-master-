#pragma once
#include"Common/JGRCCommon.h"
#include"Common/JGRCGlobalType.h"
// ���̷�Ʈ ����Ʈ �� �׸���
// 
namespace JGRC
{
	class DirectionLightPass;
	// depth ���̴� ���� ����
	// ���� ��� ���� ����
	class CORE_EXPORT LightPass
	{
	private:
		class DirectX* m_Dx;
		class JGShaderArray* m_Shader;
		std::shared_ptr<DirectionLightPass> m_DirLightPass;
	public:
		void InitPass(const DxWinConfig& config);
		void Draw(class SRSRenderTarget* SRSRT, class Camera* cam, class JGViewport* viewport);
	};
}
