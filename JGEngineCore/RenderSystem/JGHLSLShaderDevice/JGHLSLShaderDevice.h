#pragma once
#include"../../EngineStatics/Engine.h"
#include"JGShader.h"

class JGDeviceD;
class JGBufferManager;
class JGTexture;

/*
Class : JGHSLSLShaderDevice
@m map<const wstring, shared_ptr<JGShader>> m_mShaders : ���̴� �迭 */
class ENGINE_EXPORT JGHLSLShaderDevice
{
private:
	std::map<const std::wstring, std::shared_ptr<JGShader>> m_mShaders;

	JGDeviceD*       m_pDevice;
	JGBufferManager* m_pBufferManager;
public:
	JGHLSLShaderDevice();
	~JGHLSLShaderDevice();
	/*
	Exp : JGHLSLShaderDevice �� �����Ѵ�. */
	bool CreateDevice(JGDeviceD* Device, JGBufferManager* BufferManager);
	/*
	Exp : Ư�� ���̴��� ������ ��Ų��. 
	@param const wstring name       : ���۽�ų ���̴� �̸� 
	@param void*         Data       : ���� ��ų �Ķ���� ������
	@param JGTexture*    Texture    : ������ų �ؽ��� Ŭ���� 
	@param size_t        IndexCount : �������� ���� ����   */
	void Render(const std::wstring& name, JGShaderData* Data, JGTexture* Texture, size_t IndexCount);


	/*
	Exp : ���̴��� �߰��Ѵ�. 
	@template Shadertype : �߰��� ���̴� Ÿ��
	@param const wstring name : �߰��� ���̴� �̸� */
	template<class Shadertype>
	bool AddShader(const std::wstring& name);

	JGShader* GetShader(const std::wstring& name);


	static JGHLSLShaderDevice* GetInstance();
};


template<class Shadertype>
bool JGHLSLShaderDevice::AddShader(const std::wstring& name)
{
	std::unique_ptr<JGShader> shader = std::make_unique<Shadertype>();

	bool result = shader->Create(m_pDevice, m_pBufferManager);
	if (!result)
	{
		return false;
	}
	m_mShaders.insert(std::pair <const std::wstring,
		std::unique_ptr<JGShader>>(name, move(shader)));
	return true;
}