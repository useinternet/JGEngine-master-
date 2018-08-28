#pragma once
#include"../EngineStatics/Engine.h"



/* 
Class : JGDeviceD
@m ComPtr<ID3D11Device>              m_Device                : D3D ��ġ
@m ComPtr<ID3D11DeviceContext>       m_DeviceContext         : D3D ��ġ Context
@m unique_ptr<class VideoDeviceInfo> m_VideoDeviceInfomation : ���� ��ġ ����
*/
class ENGINE_EXPORT JGDeviceD
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device>        m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	std::unique_ptr<class VideoDeviceInfo>      m_VideoDeviceInfomation;
public:
	JGDeviceD();
	~JGDeviceD();
	/* 
	Exp : ��ġ�� �����Ѵ�.*/
	bool CreateDevice();
	/*
	Exp : ID3D11Device �����͸� ����Ѵ� */
	ID3D11Device* GetDevice();
	/*
	Exp : ID3D11DeviceContext �����͸� ����Ѵ� */
	ID3D11DeviceContext* GetContext();
	/*
	Exp : ���� ��ġ ������ ����Ѵ� */
	VideoDeviceInfo* GetVideoDeviceInformation();

	/*
	Exp : �ν��Ͻ��� �����´�.*/
	static JGDeviceD* GetInstance();
};





