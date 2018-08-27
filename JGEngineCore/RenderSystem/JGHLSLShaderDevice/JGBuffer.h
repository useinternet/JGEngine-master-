#pragma once
#include"../../EngineStatics/Engine.h"

class JGDeviceD;
/*
EnumClass : EJGBufferType
@e ConstantBuffer :  ��� ����
@e VertexBuffer   :  ���� ����
@e IndexBuffer    :  �ε��� ����
@e InstanceBuffer :  �ν��Ͻ� ����
*/
enum class ENGINE_EXPORT EJGBufferType
{
	ConstantBuffer  = 0,
	VertexBuffer   = 1,
	IndexBuffer    = 2,
	InstanceBuffer = 3
};
/*
EnumClass : EJGUsageType
@e Static  : ������ Ÿ��
@e Dynamic : ������ Ÿ��
*/
enum class ENGINE_EXPORT EJGUsageType
{
	Static  = 0,
	Dynamic = 1
};
/*
EnumClass : EJGCPUType
@e Access_Write : ����� ����
@e Access_Read  : �б�� ����
@e None         : ����X
*/
enum class ENGINE_EXPORT EJGCPUType
{
	Access_Write = 0,
	Access_Read  = 1,
	None         = 2
};
/*
EnumClass : EJGMapType
@e Read  :  �б� ���
@e Write :  ���� ���
@e Read_Write : �б� ���� ��� ���
@e Write_Discard : ���� ��� ( ���� �����͸� ��� �����.)
@e Write_NoOverWrite : ���� ���( ���� �����͸� ������ �ʴ´�.)
*/
enum class ENGINE_EXPORT EJGMapType
{
	Read              = 0,
	Write             = 1,
	Read_Write        = 2,
	Write_Discard     = 3,
	Write_NoOverWrite = 4
};
/*
Class : JGBuffer
@m ComPtr<ID3D11Buffer> m_Buffer     : ����
@m D3D11_BUFFER_DESC    m_BufferDesc : ���� ���� ���
@m EJGBufferType        m_BufferType : ���� Ÿ��
@m size_t m_BindDataSize : ���ε��� ������ ũ��
*/
class ENGINE_EXPORT JGBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
	D3D11_BUFFER_DESC m_BufferDesc;
	EJGBufferType     m_BufferType;


	size_t m_BindDataSize;
	JGDeviceD* m_pDevice = nullptr;
public:
	JGBuffer();
	~JGBuffer();
	/*
	Exp : ���� ����
	@param JGDeviceD* Device : DirectX ��ġ
	@param EJGBufferType bufferType : ���� Ÿ��
	@param EJGUsageType usage : ���� �뵵
	@param EJGCPUType CPUtype : CPU ��� ���� (�б�, ����)
	@param void* data : ���ε��� ������
	@param UINT size : ������ ������ ( sizeof(TYPE) * ��� ���� )  */
	bool CreateBuffer(JGDeviceD* Device, EJGBufferType bufferType, EJGUsageType usage, EJGCPUType CPUtype,
		void* data, size_t size);
	/*
	Exp : ������ ���� 
	@param : EJGMapType type : �� Ÿ�� 
	@param : void* InData : ��ü�� ������ */
	bool Write(EJGMapType type,void* InData);
	/*
	Exp : �̱��� */
	bool Read(EJGMapType type, void* OutData);
	/*
	Exp : ������ �����͸� ����Ѵ�. */
	ID3D11Buffer* Get();
	/*
	Exp : ������ ������ �ּҸ� ����Ѵ�. */
	ID3D11Buffer** GetAddress();
	/*
	Exp : ����Ÿ���� ���´�.*/
	EJGBufferType GetType();
	/*
	Exp : ���ۼ�������� �����´�. */
	void GetDesc(D3D11_BUFFER_DESC& Desc);
	/*
	Exp : ���ۿ� ���ε��� �������� ����� �����´�. */
	size_t GetBindingDataSize();
};


