#pragma once
#include"../../EngineStatics/Engine.h"



class JGBuffer;
class JGDeviceD;
enum class EJGBufferType;
enum class EJGUsageType;
enum class EJGCPUType;
enum class EJGMapType;


/*
Class : JGBufferManager 
@m BufferArray m_JGBuffers : ���� �迭(std::map)  */
class ENGINE_EXPORT JGBufferManager
{
private:
	typedef std::map<const std::wstring, std::shared_ptr<JGBuffer>> BufferArray;
private:
	BufferArray m_JGBuffers;

	JGDeviceD* m_pDevice = nullptr;
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
	@param EJGBufferType bufferType : ���� Ÿ��
	@param EJGUsageType usage : ���� �뵵
	@param EJGCPUType CPUtype : CPU ��� ���� (�б�, ����)
	@param void* data : ���ε��� ������
	@param size_t size : ������ ������ ( sizeof(TYPE) * ��� ���� )  */
	bool AddBuffer(const std::wstring& name, EJGBufferType bufferType, EJGUsageType usage, EJGCPUType CPUtype,
		void* data, size_t size);
	/*
	Exp : ���� ����
	@param const std::wstring& name : ���� �̸�
	@param : EJGMapType type : �� Ÿ��
	@param : void* InData : ��ü�� ������ */
	bool Write(const std::wstring& name, EJGMapType type, void* InData);
	/*
	Exp : �ش� �̸��� ���۸� ���´�.
	@param const std::wstring& name : ���� �̸� */
	JGBuffer* GetBuffer(const std::wstring& name);
	/*
	Exp : ���ε��� �������� ũ�⸦ �����´�. */
	size_t GetBindingDataSize(const std::wstring& name);
	/*
	Exp : ���� ���� �Ŵ����� ��ϵǾ����ִ� ���� �̸� ���� üũ�Ѵ�. */
	bool DuplicateCheck(const std::wstring& name);

	/*
	Exp : �ش� ���۸� �����Ѵ�. */
	void DestroyBuffer(const std::wstring& BufferName);
};