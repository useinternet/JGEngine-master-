#pragma once
#include"Common/JGRCCommon.h"
#include"DirectXCommon.h"

/*
// �ӽ� ����
m_Device->GetContext()->OMSetRenderTargets(1, m_RenderTarget->GetAddress(),
m_RenderTarget->GetDepthStencilView());
m_Device->GetContext()->OMSetDepthStencilState(m_RenderState->GetDepthState(EDepthStateType::ZBufferOn), 1);
m_Device->GetContext()->RSSetState(m_RenderState->GetRasterizerState(EFillModeType::Solid));
m_Device->GetContext()->RSSetViewports(1, m_Viewport->Get());
*/

namespace JGRC
{
	class JGRenderTarget;
	class JGRenderState;
	class JGViewport;
	class CORE_EXPORT JGDevice
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Device>        m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	public:
		JGDevice();
		~JGDevice();
		/*
		Exp : ��ġ�� �����Ѵ�.*/
		bool CreateDevice();
		/*
		Exp : ���� Ÿ�� �並 ��ġ�� ���ε� �մϴ�*/
		void BindRederTarget(JGRenderTarget* renderTarget, const UINT numViews = 1);
		/*
		Exp : ���� ���ٽ� ���¸� ���ε� �Ѵ�. */
		void BindDepthStencilState(JGRenderState* state, const EDepthStateType type, const UINT numDepths = 1);
		/*
		Exp : ���� ���¸� ���ε� �Ѵ�. */
		void BindBlendState(JGRenderState* state, const EBlendStateType type, const float blendFactor[4], const UINT SampleMask = 0xffffffff);
		/*
		Exp : ������ ���¸� ���ε� �Ѵ�. */
		void BindRasterizerState(JGRenderState* state,const EFillModeType type);
		/*
		Exp : ����Ʈ�� ���ε��Ѵ�. */
		void BindViewport(JGViewport* viewport, const UINT numVps = 1);

		/*
		Exp : ID3D11Device �����͸� ����Ѵ� */
		ID3D11Device* GetDevice();
		/*
		Exp : ID3D11DeviceContext �����͸� ����Ѵ� */
		ID3D11DeviceContext* GetContext();

	
	};


}






