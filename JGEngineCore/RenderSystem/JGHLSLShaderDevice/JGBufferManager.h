#pragma once
#include"../../EngineStatics/Engine.h"



class JGBuffer;
class JGDeviceD;
enum class JGBufferType;
enum class JGUsageType;
enum class JGCPUType;
enum class JGMapType;


/*
Class : JGBufferManager 
@m BufferArray m_JGBuffers : ���� �迭(std::map)  */
class ENGINE_EXPORT JGBufferManager
{
private:
	typedef std::map<const std::wstring, std::shared_ptr<JGBuffer>> BufferArray;
private:
	BufferArray m_JGBuffers;
	JGDeviceD* m_pDevice;

public:
	JGBufferManager();
	JGBufferManager(JGBufferManager& copy) = delete;
	~JGBufferManager();
	/*
	Exp : ���� �Ŵ��� �ʱ�ȭ 
	@param JGDeviceD* Device : DirectX ��ġ */
	void InitBufferManager(JGDeviceD* Device);
	/*
	Exp : ���� �߰� 
	@param const std::wstring& name : ���� �̸�
	@param JGBufferType bufferType : ���� Ÿ��
	@param JGUsageType usage : ���� �뵵
	@param JGCPUType CPUtype : CPU ��� ���� (�б�, ����)
	@param void* data : ���ε��� ������
	@param UINT size : ������ ������ ( sizeof(TYPE) * ��� ���� )  */
	bool AddBuffer(const std::wstring& name, JGBufferType bufferType, JGUsageType usage, JGCPUType CPUtype,
		void* data, UINT size);
	/*
	Exp : ���� ����
	@param const std::wstring& name : ���� �̸�
	@param : JGMapType type : �� Ÿ��
	@param : void* InData : ��ü�� ������ */
	bool Write(const std::wstring& name, JGMapType type, void* InData);

	/*
	Exp : �ش� �̸��� ���۸� ���´�.
	@param const std::wstring& name : ���� �̸� */
	JGBuffer* GetBuffer(const std::wstring& name);
};