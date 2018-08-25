#pragma once
#include"../EngineStatics/Engine.h"


typedef DXGI_ADAPTER_DESC1          AdapterDesc;
typedef DXGI_MODE_DESC              DisplayModeDesc;

/*
Class : VideoDeviceInfo
@m AdapterDesc     m_AdaterDesc : ���� ī��
@m DisplayModeList m_ModeList   : ���÷��� ��� ���
*/
class ENGINE_EXPORT VideoDeviceInfo
{
	typedef std::vector<DisplayModeDesc> DisplayModeList;
private:
	AdapterDesc     m_AdaterDesc;
	DisplayModeList m_ModeList;
public:
	/*
	Exp : ������ġ ������ Ž���մϴ�.*/
	bool SearchVideoDeviceInformation();
	/*
	Exp : ���� ī�� ������ �����ɴϴ�.*/
	AdapterDesc& GetVideoCard();
	/*
	Exp : ���÷��� ��� ��Ͽ��� ȭ�鿡 �´� ��� ������ �����ɴϴ�. */
	DisplayModeDesc& GetDisplayModeList(const int ScreenWidth, const int ScreenHeight);
};
