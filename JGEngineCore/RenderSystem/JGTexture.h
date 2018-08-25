#pragma once
#include"../EngineStatics/Engine.h"


//
class JGDeviceD;


/*
Class : JGTexture
@m vector<ID3D11ShaderResourceView*> m_vTexture     : �ؽ��� �迭
@m vector<D3DX11_IMAGE_INFO>         m_vTextureInfo : �ؽ��� ���� �迭
*/
class ENGINE_EXPORT JGTexture
{
private:
	 std::vector<ID3D11ShaderResourceView*> m_vTexture;
	 std::vector<D3DX11_IMAGE_INFO> m_vTextureInfo;
public:
	JGTexture();
	~JGTexture();
	/*
	Exp : �ؽ��� �߰�
	@param JGDeviceD* Device   : DirectX ��ġ
	@param const wstring& Path : �ؽ��� ���丮 ���  */
	bool Add(JGDeviceD* Device,const std::wstring& Path);
	/*
	Exp : �ؽ��ĸ� ���´�.
	@param const UINT idx : ���� �ؽ��� �ε��� */
	ID3D11ShaderResourceView* Get(const UINT idx);
	/*
	Exp : �ؽ��Ĺ迭�� ���ۺκ��� ���´�. */
	ID3D11ShaderResourceView* GetEntry();
	/*
	Exp : �ؽ��������� ���´�.
	@param const UINT idx : ���� �ؽ������� �ε��� */
	D3DX11_IMAGE_INFO& GetInformation(const UINT idx);
};