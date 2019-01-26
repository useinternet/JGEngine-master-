#pragma once
#include"DxCommon/DxCommon.h"
#include"DxSetting.h"
class Camera;


// �ν��Ͻ� ���� ���
// �Ǻ� �� -> �Ǻ� �� �͸� ������Ʈ(UpdateNotify -- �߰� )
namespace JGRC
{
	class DxCore;


	class RCORE_EXPORT JGRenderCore
	{
	private:
		std::unique_ptr<DxCore>      m_DxCore;
		std::unique_ptr<class Scene> m_Scene;

		class JGLight* DirLight = nullptr;
		class JGLight* PointLight[4];
		class JGLight* SpotLight[4];
		bool PointSwitch[4];
		
		Camera* m_Camera;
		HWND hWnd;
	private:
		JGRenderCore(const JGRenderCore& copy) = delete;
		JGRenderCore& operator=(const JGRenderCore& copy) = delete;
		void InputCamera(const GameTimer& gt);
		void BuildTextures();
		void BuildLight();
		void BuildLandGeometry();
		void DrawRenderItems();
	public:
		JGRenderCore();
		~JGRenderCore();

		bool Init(const DxSetting& set);
		void Update(const GameTimer& Timer);
		void ScreenReSize(int width, int height);
		void UpdateLook(WPARAM btnState, int x, int y, POINT& mLastMousePos);
		void Draw();
	};
}
