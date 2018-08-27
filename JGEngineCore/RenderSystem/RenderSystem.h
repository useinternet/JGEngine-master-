#pragma once
#include"../EngineStatics/Engine.h"

/*
Class Name : RenderSystem
@m std::unique_ptr<class JGDeviceD>            m_Device       : Direct ��ġ
@m std::unique_ptr<class JGSwapChainD>         m_SwapChain    : ����ü��
@m std::unique_ptr<class JGRenderTargetD>      m_RenderTarget : ���� ����Ÿ��
@m std::unique_ptr<class JGRenderSystemStateD> m_RenderState  : �������ý��� ����
@m std::unique_ptr<class JGViewportD>          m_Viewport     : ����Ʈ
@m std::unique_ptr<class JGBufferManager>      m_JGBufferManager : ���� �Ŵ���( �������, ���� ����, �ε��� ����)
@m std::unique_ptr<class JBHLSLShaderDevice>   m_ShaderDevice : hlsl ��ġ( ���̴� ���� ��ġ)
@m std::unique_ptr<class JGShaderConstructor>  m_ShaderConstructor : ���̴� ���� ���� Ŭ����
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
	std::unique_ptr<class JGBufferManager>      m_JGBufferManager;
	std::unique_ptr<class JGHLSLShaderDevice>   m_ShaderDevice;
	std::unique_ptr<class JGShaderConstructor>  m_ShaderConstructor;
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

	/*
	Exp : �׸���  */
	void Render();


	//�ӽ�
	void TestFunc(HWND hWnd);
	class JGMesh* m_Mesh;
	class JGTexture* texture;
	D3DXMATRIX ViewMatrix;
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 tex;
	};
	//


private:
	void ApplicationInDeviceContext();
};