#pragma once
#include"../../EngineStatics/Engine.h"

class JGDeviceD;
/*
EnumClass : JGBufferType
@e ConstantBuffer :  ��� ����
@e VertexBuffer   :  ���� ����
@e IndexBuffer    :  �ε��� ����
@e InstanceBuffer :  �ν��Ͻ� ����
*/
enum class ENGINE_EXPORT JGBufferType
{
	ConstantBuffer  = 0,
	VertexBuffer   = 1,
	IndexBuffer    = 2,
	InstanceBuffer = 3
};
/*
EnumClass : JGUsageType
@e Static  : ������ Ÿ��
@e Dynamic : ������ Ÿ��
*/
enum class ENGINE_EXPORT JGUsageType
{
	Static  = 0,
	Dynamic = 1
};
/*
EnumClass : JGCPUType
@e Access_Write : ����� ����
@e Access_Read  : �б�� ����
@e None         : ����X
*/
enum class ENGINE_EXPORT JGCPUType
{
	Access_Write = 0,
	Access_Read  = 1,
	None         = 2
};
/*
EnumClass : JGMapType
@e Read  :  �б� ���
@e Write :  ���� ���
@e Read_Write : �б� ���� ��� ���
@e Write_Discard : ���� ��� ( ���� �����͸� ��� �����.)
@e Write_NoOverWrite : ���� ���( ���� �����͸� ������ �ʴ´�.)
*/
enum class ENGINE_EXPORT JGMapType
{
	Read              = 0,
	Write             = 1,
	Read_Write        = 2,
	Write_Discard     = 3,
	Write_NoOverWrite = 4
};
/*
Class : JGBuffer
@m ComPtr<ID3D11Buffer> m_Buffer  : ����
@m D3D11_BUFFER_DESC m_BufferDesc : ���� ���� ���
@m JGBufferType      m_BufferType : ���� Ÿ��
*/
class ENGINE_EXPORT JGBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
	D3D11_BUFFER_DESC m_BufferDesc;
	JGBufferType     m_BufferType;
	JGDeviceD* m_pDevice = nullptr;
public:
	JGBuffer();
	~JGBuffer();
	/*
	Exp : ���� ����
	@param JGDeviceD* Device : DirectX ��ġ
	@param JGBufferType bufferType : ���� Ÿ��
	@param JGUsageType usage : ���� �뵵
	@param JGCPUType CPUtype : CPU ��� ���� (�б�, ����)
	@param void* data : ���ε��� ������
	@param UINT size : ������ ������ ( sizeof(TYPE) * ��� ���� )  */
	bool CreateBuffer(JGDeviceD* Device, JGBufferType bufferType, JGUsageType usage, JGCPUType CPUtype,
		void* data, UINT size);
	/*
	Exp : ������ ���� 
	@param : JGMapType type : �� Ÿ�� 
	@param : void* InData : ��ü�� ������ */
	bool Write(JGMapType type,void* InData);
	/*
	Exp : �̱��� */
	bool Read(JGMapType type, void* OutData);
	/*
	Exp : ������ �����͸� ����Ѵ�. */
	ID3D11Buffer* Get();
	/*
	Exp : ������ ������ �ּҸ� ����Ѵ�. */
	ID3D11Buffer** GetAddress();
	/*
	Exp : ����Ÿ���� ���´�.*/
	JGBufferType GetType();
	/*
	Exp : ���ۼ�������� �����´�. */
	void GetDesc(D3D11_BUFFER_DESC& Desc);
};


