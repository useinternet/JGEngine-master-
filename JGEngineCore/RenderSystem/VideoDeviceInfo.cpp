#include"VideoDeviceInfo.h"
using namespace Microsoft::WRL;


bool VideoDeviceInfo::SearchVideoDeviceInformation()
{
	HRESULT result = S_OK;
	ComPtr<IDXGIFactory1> Factory(nullptr);
	ComPtr<IDXGIAdapter1> Adapter(nullptr);
	ComPtr<IDXGIOutput>   Output(nullptr);

	// DXGIFactory1 ����
	result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)Factory.GetAddressOf());
	if (FAILED(result))
	{
		//���� �α����
		return false;
	}


	bool bFind = false;
	// VideoCard ã��
	for (UINT i = 0; Factory->EnumAdapters1(i, Adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		// Display ã��
		for (UINT j = 0; Adapter->EnumOutputs(j, Output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++j)
		{
			UINT modesNum = 0;
			// ���÷��� ��� ã��
			for (UINT k = 0; k <= DXGI_FORMAT_BC7_UNORM_SRGB; ++k)
			{
				DXGI_FORMAT format = static_cast<DXGI_FORMAT>(k);

				Output->GetDisplayModeList(format, 0, &modesNum, nullptr);

				if (modesNum == 0) continue;

				// ���� ������
				m_ModeList.reserve(modesNum);
				Output->GetDisplayModeList(format, 0, &modesNum, &m_ModeList[0]);
				Adapter->GetDesc1(&m_AdaterDesc);


				// ã������ Loop break
				bFind = true;
				break;
			}
		}
	}
	// ã�� �������� false ��ȯ
	if (!bFind)
	{
		// ���� �α� ���
		return false;
	}
	// ���� �α� ���
	return true;
}

AdapterDesc& VideoDeviceInfo::GetVideoCard()
{
	return m_AdaterDesc;
}

DisplayModeDesc& VideoDeviceInfo::GetDisplayModeList(const int ScreenWidth, const int ScreenHeight)
{
	for (size_t i = 0; i < m_ModeList.size(); ++i)
	{
		if (m_ModeList[i].Width == ScreenWidth && m_ModeList[i].Height == ScreenHeight)
		{
			return m_ModeList[i];
		}
	}

	// ���� �α� ���
	return m_ModeList.at(0);
}
