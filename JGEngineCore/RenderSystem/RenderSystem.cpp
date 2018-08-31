#include"RenderSystem.h"
#include"JGDeviceD.h"
#include"VideoDeviceInfo.h"
#include"JGSwapChainD.h"
#include"JGRenderTargetD.h"
#include"JGRenderSystemStateD.h"
#include"JGViewportD.h"
#include"../EngineStatics/JGLog.h"
#include"JGHLSLShaderDevice/JGBufferManager.h"
#include"JGHLSLShaderDevice/JGHLSLShaderDevice.h"
#include"JGShaderConstructor.h"
#include"JGRenderSuperClass.h"
#include"../EngineFrameWork/Object/Object.h"
#include"../EngineFrameWork/World/WorldCompare.h"

// �ӽ� ��Ŭ���
#include"../EngineFrameWork/Components/StaticMesh2DComponent.h"
#include"../EngineFrameWork/Object/ExistObject.h"

using namespace std;

RenderSystem::RenderSystem()
{
	m_Device       = make_unique<JGDeviceD>();
	m_SwapChain    = make_unique<JGSwapChainD>();
	m_RenderTarget = make_unique<JGRenderTargetD>();
	m_RenderState  = make_unique<JGRenderSystemStateD>();
	m_Viewport     = make_unique<JGViewportD>();
	m_JGBufferManager = make_unique<JGBufferManager>();
	m_ShaderDevice = make_unique<JGHLSLShaderDevice>();
	m_ShaderConstructor = make_unique<JGShaderConstructor>();
	m_SuperClass = make_unique<JGRenderSuperClass>();
	m_ObjectConstructInit = make_unique<Object>();
}
RenderSystem::~RenderSystem() {}
bool RenderSystem::InitRenderSystem(HWND hWnd, const bool bFullScreen,const int ScreenWidth, const int ScreenHeight,
	const float FOV, const float FarZ, const float NearZ)
{
	// ���� �ʱ�ȭ
	m_hWnd = hWnd;
	m_bFullScreen  = bFullScreen;
	m_ScreenWidth  = ScreenWidth;
	m_ScreenHeight = ScreenHeight;
	bool result = true;
	// �α� ���( ���� )
	JGLog::Write(ELogLevel::Progress, TT("RenderSystem Init Start...."));

	// ��ġ ����
	JGLog::Write(ELogLevel::Default, TT("JGDeviceD Init Start...."));
	result = m_Device->CreateDevice();
	if (!result)
	{
		JGLog::Write(ELogLevel::Error, TT("Create JGDeviceD Failed."));
		return false;
	}
	JGLog::Write(ELogLevel::Default, TT("JGDeviceD Init Complete...."));

	// ����ü�� ����
	JGLog::Write(ELogLevel::Default, TT("SwapChain Init Start...."));
	result = m_SwapChain->CreateSwapChain(m_Device->GetDevice(), m_hWnd, m_bFullScreen, m_ScreenWidth, m_ScreenHeight);
	if (!result)
	{
		JGLog::Write(ELogLevel::Error, TT("Create JGSwapChainD Failed."));
		return false;
	}
	JGLog::Write(ELogLevel::Default, TT("SwapChain Init Complete...."));
	// �α� ��� ���� �Ϸ�

	// ���� Ÿ�� ����
	JGLog::Write(ELogLevel::Default, TT("MainJGRenderTargetD Init Start...."));
	result = m_RenderTarget->CreateRenderTarget(m_Device->GetDevice(), m_SwapChain->Get(), m_ScreenWidth, m_ScreenHeight);
	if (!result)
	{
		JGLog::Write(ELogLevel::Error, TT("Create MainJGRenderTargetD Failed."));
		return false;
	}
	JGLog::Write(ELogLevel::Default, TT("MainJGRenderTargetD Init Complete...."));

	// �����ý��� ���� ����
	JGLog::Write(ELogLevel::Default, TT("JGRenderSystemStateD Init Start...."));
	result = m_RenderState->InitRenderSystemSetting(m_Device->GetDevice());
	if (!result)
	{
		JGLog::Write(ELogLevel::Error, TT("Create JGRenderSystemStateD Failed."));
		return false;
	}
	JGLog::Write(ELogLevel::Default, TT("JGRenderSystemStateD Init Complete...."));


	// ����Ʈ ����
	JGLog::Write(ELogLevel::Default, TT("JGMainViewport Init Start...."));
	result = m_Viewport->InitViewport(m_ScreenWidth, m_ScreenHeight, FOV, FarZ, NearZ);
	if (!result)
	{
		JGLog::Write(ELogLevel::Error, TT("Create JGMainViewport Failed."));
		return false;
	}
	JGLog::Write(ELogLevel::Default, TT("JGMainViewport Init Complete...."));


	JGLog::Write(ELogLevel::Default, TT("RenderSystemAssist Connecting..."));
	 //���� �Ŵ��� �ʱ�ȭ
	m_JGBufferManager->InitBufferManager(m_Device.get());
	// ���̴� �ý��� �ʱ�ȭ
	m_ShaderDevice->CreateDevice(m_Device.get(), m_JGBufferManager.get());
	 //���̴� ���� �����ڿ� ���̴� �ý��۰� ����
	m_ShaderConstructor->LinkHLSLDevice(m_ShaderDevice.get());
	JGLog::Write(ELogLevel::Default, TT("RenderSystemAssist ConnectComplete..."));


	// ������ ���� Ŭ���� ����
	m_SuperClass->LinkPointer(m_Device.get(), m_Viewport.get(), m_ShaderDevice.get(), m_JGBufferManager.get());
	// ������Ʈ�� ���������ʿ��� ������ ����
	m_ObjectConstructInit->InitObejct(m_SuperClass.get());


	// �ӽ�
	// �ӽ� ����
	ApplicationInDeviceContext();

	//
	JGLog::Write(ELogLevel::Progress, TT("RenderSystem Init Complete...."));
	return true;
}

void RenderSystem::BeginRendering()
{
	float Color[4] = { 0.0f,0.0f,0.0f,0.0f};
	m_Device->GetContext()->ClearRenderTargetView(m_RenderTarget->Get(), Color);
	m_Device->GetContext()->ClearDepthStencilView(m_RenderTarget->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void RenderSystem::EndRendering()
{
	m_SwapChain->Get()->Present(0, 0);
}
void RenderSystem::Render()
{
	BeginRendering();
	float BlendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };
	
	// ���� �� Z���� ����
	m_Device->GetContext()->OMSetBlendState(
		m_RenderState->GetBlendState(EBlendStateType::BlendOn),BlendFactor, 0xffffffff);
	m_Device->GetContext()->OMSetDepthStencilState(m_RenderState->GetDepthState(EDepthStateType::ZBufferOff), 1);

	m_RenderingObjectArray->sort([](
		shared_ptr<Object>& object1, shared_ptr<Object>& object2)->bool
	{
		if (object1->GetZOrder() < object2->GetZOrder())
		{
			return true;
		}
		return false;
	});


	// ������
	for (auto& object : *m_RenderingObjectArray)
	{
		if (object->GetObjectState() == EObjectState::Active)
		{
			object->Render();
		}
	}

	// ���� off �� Z���� ����
	m_Device->GetContext()->OMSetDepthStencilState(m_RenderState->GetDepthState(EDepthStateType::ZBufferOn), 1);
	m_Device->GetContext()->OMSetBlendState(
		m_RenderState->GetBlendState(EBlendStateType::BlendOff), BlendFactor, 0xffffffff);

	EndRendering();
}

void RenderSystem::ReceiveObjectArray(list<shared_ptr<Object>>* Array)
{
	m_RenderingObjectArray = Array;
}

void RenderSystem::ApplicationInDeviceContext()
{
	// �ӽ� ����
	m_Device->GetContext()->OMSetRenderTargets(1, m_RenderTarget->GetAddress(),
		m_RenderTarget->GetDepthStencilView());
	m_Device->GetContext()->OMSetDepthStencilState(m_RenderState->GetDepthState(EDepthStateType::ZBufferOn), 1);
	m_Device->GetContext()->RSSetState(m_RenderState->GetRasterizerState(EFillModeType::Solid));
	m_Device->GetContext()->RSSetViewports(1, m_Viewport->Get());
}
