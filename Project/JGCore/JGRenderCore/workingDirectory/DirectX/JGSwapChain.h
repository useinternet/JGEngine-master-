#pragma once
#include"Common/JGRCCommon.h"

namespace JGRenderCore
{
	class CORE_EXPORT JGSwapChain
	{
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
	public:
		JGSwapChain();
		~JGSwapChain();
		/*
		Exp : ����ü���� �����Ѵ�.
		@param ID3D11Device* Device : Direct ��ġ
		@param HWND hWnd : ������ �ڵ�
		@param const bool bFullScreen : ��ü ȭ�� ����
		@param const int ScreenWidth  : ��ũ�� ���� ����
		@param const int ScreenHeight : ��ũ�� ���� ����
		*/
		bool CreateSwapChain(ID3D11Device* Device, HWND hWnd, const bool bFullScreen,
			const int ScreenWidth, const int ScreenHeight);

		/*
		Exp : ����ü�� �����͸� ����Ѵ�.*/
		IDXGISwapChain* Get();
	private:
	};
}

