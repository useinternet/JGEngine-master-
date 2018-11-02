#include"JGDevice.h"
#include"JGRenderTarget.h"
#include"JGRenderState.h"
#include"JGViewport.h"
using namespace JGRC;
using namespace std;
JGDevice::JGDevice()
{
	m_Device = nullptr;
	m_DeviceContext = nullptr;
}
JGDevice::~JGDevice() {}
bool JGDevice::CreateDevice()
{
	JGLOG(log_Info, "JGRC::JGDevice", "Createing JGDevice..");
	UINT CreateDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#else
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
#endif 

	HRESULT result = S_OK;

	const size_t LevelCount = 6;
	D3D_FEATURE_LEVEL FeatureLevel[LevelCount] =
	{
		D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
	};
	D3D_FEATURE_LEVEL HighestLevel = D3D_FEATURE_LEVEL_11_0;


	// ��ġ ����
	result = D3D11CreateDevice(nullptr, 
		D3D_DRIVER_TYPE_HARDWARE, nullptr,
		CreateDeviceFlags, 
		FeatureLevel, LevelCount,
		D3D11_SDK_VERSION, 
		m_Device.GetAddressOf(),
		&HighestLevel,
		m_DeviceContext.GetAddressOf()
	);
	if (FAILED(result))
	{
		JGLOG(log_Critical, "JGRC::JGDevice", "Failed JGDevice..");
		return false;
	}
	if (HighestLevel != D3D_FEATURE_LEVEL_11_0)
	{
		JGLOG(log_Error, "JGRC::JGDevice", "HighestLevel is not D3D_FEATURE_LEVEL_11_0..");
		// ����ó�� �α� ���
	}

	JGLOG(log_Info, "JGRC::JGDevice", "Create JGDevice Complete.");
	return true;
}
void JGDevice::BindRederTarget(JGRenderTarget* renderTarget, const UINT numViews)
{
	m_DeviceContext->OMSetRenderTargets(numViews, renderTarget->GetAddress(), renderTarget->GetDepthStencilView());
}
void JGDevice::BindDepthStencilState(JGRenderState* state, const EDepthStateType type, const UINT numDepths)
{
	m_DeviceContext->OMSetDepthStencilState(state->GetDepthState(type), numDepths);
}
void JGDevice::BindBlendState(JGRenderState* state, const EBlendStateType type, const float blendFactor[4], const UINT SampleMask)
{
	m_DeviceContext->OMSetBlendState(state->GetBlendState(type), blendFactor, SampleMask);
}
void JGDevice::BindRasterizerState(JGRenderState* state, const EFillModeType type)
{
	m_DeviceContext->RSSetState(state->GetRasterizerState(type));
}
void JGDevice::BindViewport(JGViewport* viewport, const UINT numVps)
{
	m_DeviceContext->RSSetViewports(numVps, viewport->Get());
}
ID3D11Device* JGDevice::GetDevice()
{
	if (m_Device.Get())
	{
		return m_Device.Get();
	}
	// ���� ���
	JGLOG(log_Error, "JGRC::JGDevice", "Device is nullptr.");
	return nullptr;
}
ID3D11DeviceContext* JGDevice::GetContext()
{
	if (m_DeviceContext.Get())
	{
		return m_DeviceContext.Get();
	}
	// ���� ���
	JGLOG(log_Error, "JGRC::JGDevice", "DeviceContext is nullptr.");
	return nullptr;
}

