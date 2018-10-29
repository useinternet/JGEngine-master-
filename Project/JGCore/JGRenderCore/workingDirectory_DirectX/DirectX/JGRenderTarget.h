#pragma once
#include"Common/JGRCCommon.h"
#include"DirectXCommon.h"
// �߰��Ұ�
// ����Ÿ�� ��� ����-���ٽ� ���۸� ���� ��� ���������ο� ���ε��մϴ�.
//m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());


namespace JGRC
{
	/*
	Enum Class : ERenderTextureTypew
	@e RenderTarget : ���� Ÿ��� �ؽ���
	@e DepthBuffer  : ���� ���ۿ� �ؽ���
	*/
	enum class CORE_EXPORT ERenderTextureType
	{
		RenderTarget = 0,
		DepthBuffer = 1
	};
	/*
	Class : JGRenderTargetD
	@m RenderTargetView   m_RenderTarget      : ���� Ÿ��
	@m ShaderResourceView m_ShaderResource    : ���̴� ���ҽ� ��(���� �ؽ���)
	@m Texture2DArray     m_D3DTexture2D      : �ؽ��� �迭 (�׸� ��ȭ����)
	@m DepthSV            m_DepthStencilView  : ���� ���ٽ� ��
	@m ERenderTargetType  m_RenderTargetType  : ���� Ŭ���� ����Ÿ�� Ÿ��
	@m int m_Width  : �׸��� �ؽ��� ���� ����
	@m int m_Height : �׸��� �ؽ��� ���� ����
	*/
	class CORE_EXPORT JGRenderTarget
	{
		typedef Microsoft::WRL::ComPtr<ID3D11RenderTargetView>        RenderTargetView;
		typedef Microsoft::WRL::ComPtr<ID3D11DepthStencilView>        DepthSV;
		typedef std::vector<Microsoft::WRL::ComPtr<ID3D11Texture2D>>  Texture2DArray;

	private:
		RenderTargetView   m_RenderTarget;
		Texture2DArray     m_D3DTexture2D;
		DepthSV            m_DepthStencilView;

		int m_Width = 0;
		int m_Height = 0;
	public:
		JGRenderTarget();
		~JGRenderTarget();
		/*
		Exp : ����Ÿ���� �����Ѵ�. ( ���� ��ġ �� )
		@param ID3D11Device*   Device     : Direct ����̽�
		@param IDXGISwapChain* SwapChain  : ����ü��
		@param const int       Width      : �׸� ���� ����
		@param const int       Height     : �׸� ���� ����
		*/
		bool CreateRenderTarget(ID3D11Device* Device, IDXGISwapChain* SwapChain, const int Width, const int Height);
		/*
		Exp : ����Ÿ�� �����͸� ����Ѵ�. */
		ID3D11RenderTargetView*    Get();
		/*
		Exp : ����Ÿ�� ������ �ּҸ� ����Ѵ�. */
		ID3D11RenderTargetView**   GetAddress();
		/*
		Exp :  �ؽ��ĸ� ��´�.
		@param : const ERenderTextureType& type : �ؽ��� Ÿ�� */
		ID3D11Texture2D*        GetD3DTexture2D(const ERenderTextureType type);
		/*
		Exp :  ���� ���ٽ� �� �����͸� ����Ѵ�. */
		ID3D11DepthStencilView*  GetDepthStencilView();
		/*
		Exp :  ���� ���ٽ� �� ������ �ּҸ� ����Ѵ�. */
		ID3D11DepthStencilView** GetDepthStencilViewAddress();

	private:
		void WriteTextureDesc(D3D11_TEXTURE2D_DESC& Desc, const DXGI_FORMAT& format,
			UINT BindFlags, D3D11_USAGE Usage = D3D11_USAGE_DEFAULT);
		void WriteDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC& Desc);
	};

}
