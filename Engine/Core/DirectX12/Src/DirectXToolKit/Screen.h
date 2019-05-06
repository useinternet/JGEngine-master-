#pragma once
#include"DirectXCommon/DirectXCommon.h"
#include"RenderTarget.h"
#include"Texture.h"


namespace Dx12
{
	class Screen
	{
		static const int BackBufferCount = 3;
	private:
		HWND m_hWnd;
		Texture  m_BackBuffer[BackBufferCount];
		uint64_t m_FrameValues[BackBufferCount];
		uint64_t m_FenceValues[BackBufferCount];
		ComPtr<IDXGISwapChain4> m_SwapChain;
		uint32_t m_Width;
		uint32_t m_Height;
		DXGI_FORMAT m_Format;
	public:
		Screen() = default;
		~Screen() = default;
	public:
		// ��ũ�� �ʱ�ȭ
		// @param hwnd : ������ �ڵ�
		// @param width, height : ��ũ�� ũ��
		// @param format : ��ũ�� Format
		void InitScreen(HWND hWnd, uint32_t width, uint32_t height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);


		// ȭ�� ũ�� �缳��
		// @param width,height : ��ũ�� ũ��
		void Resize(uint32_t width, uint32_t height);


		// ȭ�� ���� 
		// @param texture : ��ũ���� ������ �ؽ���
		// @param clearColor : �ؽ����� �⺻ ���� ��
		void Present(const Texture& texture = Texture(), const DirectX::XMFLOAT4& clearColor = { 0.0f,0.0f,0.0f,1.0f });

		// ȭ�� ���� ��������
		float AspectRadio() const;

		// ȭ�� ���� ũ�� ��������
		uint32_t GetWidth() const;

		// ȭ�� ���� ũ�� �������� 
		uint32_t GetHeight() const;

		// ȭ�� Format ��������
		DXGI_FORMAT GetFormat() const;
	private:
		void UpdateRenderTarget();
		void CreateSwapChain();
	};
}