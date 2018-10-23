#pragma once
#include"Common/JGRCCommon.h"



namespace JGRenderCore
{
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
		Exp : ID3D11Device �����͸� ����Ѵ� */
		ID3D11Device* GetDevice();
		/*
		Exp : ID3D11DeviceContext �����͸� ����Ѵ� */
		ID3D11DeviceContext* GetContext();
	};


}






