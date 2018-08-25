#pragma once
#include"../EngineStatics/Engine.h"



/*
Class Name : RenderSystem
@m std::unique_ptr<class JGDeviceD>            m_Device       : Direct ��ġ
@m std::unique_ptr<class JGSwapChainD>         m_SwapChain    : ����ü��
@m std::unique_ptr<class JGRenderTargetD>      m_RenderTarget : ���� ����Ÿ��
@m std::unique_ptr<class JGRenderSystemStateD> m_RenderState  : �������ý��� ����
@m std::unique_ptr<class JGViewportD>          m_Viewport     : ����Ʈ
@m HWND m_hWnd           : ������ �ڵ�
@m int  m_ScreenWidth    : ��ũ�� ���� ����
@m int  m_ScreenHeight   : ��ũ�� ���� ����
@m bool m_bFullScreen    : ��ü ȭ�� ����
*/
class ENGINE_EXPORT RenderSystem
{
private:
	std::unique_ptr<class JGDeviceD>            m_Device;
	std::unique_ptr<class JGSwapChainD>         m_SwapChain;
	std::unique_ptr<class JGRenderTargetD>      m_RenderTarget;
	std::unique_ptr<class JGRenderSystemStateD> m_RenderState;
	std::unique_ptr<class JGViewportD>          m_Viewport;


	HWND m_hWnd;
	int  m_ScreenWidth;
	int  m_ScreenHeight;
	bool m_bFullScreen;
public:
	RenderSystem();
	~RenderSystem();

	/*
	Exp : �������ý����� �ʱ�ȭ �Ѵ�. 
	@param HWND hWnd               : ������ �ڵ�
	@param const bool bFullScreen  : ��üȭ�� ����
	@param const int ScreenWidth   : ��ũ�� ���� ����
	@param const int ScreenHeight  : ��ũ�� ���� ����
	@param const float FOV         : ȭ��� �������� ����(Field Of View)
	@param const float FarZ        : ȭ��� �׷����� Z�� �ִ� ����
	@param const float NearZ       : ȭ��� �׷����� Z�� �ּ� ���� */ 
	bool InitRenderSystem(HWND hWnd, const bool bFullScreen, const int ScreenWidth, const int ScreenHeight,
		const float FOV, const float FarZ, const float NearZ);
	/*
	Exp : �׸��� ���� */
	void BeginRendering();
	/*
	Exp : �׸��� ���� */
	void EndRendering();


private:
	void ApplicationInDeviceContext();
};