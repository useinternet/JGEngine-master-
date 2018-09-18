#pragma once
#include"../EngineStatics/Engine.h"



class JGSuperClass;
/*
Class Name : RenderSystem
@m unique_ptr<class JGDeviceD>            m_Device              : Direct ��ġ
@m unique_ptr<class JGSwapChainD>         m_SwapChain           : ����ü��
@m unique_ptr<class JGRenderTargetD>      m_RenderTarget        : ���� ����Ÿ��
@m unique_ptr<class JGRenderSystemStateD> m_RenderState         : �������ý��� ����
@m unique_ptr<class JGViewportD>          m_Viewport            : ����Ʈ
@m unique_ptr<class JGBufferManager>      m_JGBufferManager     : ���� �Ŵ���( �������, ���� ����, �ε��� ����)
@m unique_ptr<class JBHLSLShaderDevice>   m_ShaderDevice        : hlsl ��ġ( ���̴� ���� ��ġ)
@m unique_ptr<class JGShaderConstructor>  m_ShaderConstructor   : ���̴� ���� ���� Ŭ����
@m unique_ptr<class JGSuperClass>   m_SuperClass          : �������� �ʿ��� ��ġ���� �ּҰ��� ���� Ŭ����(���޿�)
@m unique_ptr<class Object>               m_ObjectConstructInit : �ʱ�ȭ�� ������Ʈ
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
	std::unique_ptr<class Object>               m_ObjectConstructInit;
	std::list<std::shared_ptr<Object>>* m_RenderingObjectArray;
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

	void InitObjectProtoType(JGSuperClass* SuperClass);
	void ReceiveObjectArray(std::list<std::shared_ptr<Object>>* Array);

	JGDeviceD*          GetDevice();
	JGViewportD*        GetViewPort();
	JGHLSLShaderDevice* GetShaderDevice();
	JGBufferManager*    GetBufferManager();


private:
	//�ӽ�
	void ApplicationInDeviceContext();
};