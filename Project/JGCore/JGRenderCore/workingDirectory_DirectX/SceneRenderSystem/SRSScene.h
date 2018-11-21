#pragma once
#include"Common/JGRCCommon.h"
#include"DirectXCommon.h"

namespace JGRC
{
	class Camera;
	class CORE_EXPORT SRSScene
	{
	private:
		std::unique_ptr<class Mesh>  m_Mesh;
		std::unique_ptr<Camera> m_Camera;
		std::unique_ptr<jgMatrix4x4> m_worldMatrix;
		std::unique_ptr<jgMatrix4x4> m_orthoMatrix;
		std::unique_ptr<jgMatrix4x4> m_wvpMatrix;
	public:
		SRSScene();
		~SRSScene();
		void CreateScene(const DxWinConfig& config);
	
		void Render();
		uint GetIndexCount();
		Camera* GetCamera();
		jgMatrix4x4& GetwvpMatrix();
		void SetwvpMatrix(const jgMatrix4x4& wvpMatrix);
	};
}