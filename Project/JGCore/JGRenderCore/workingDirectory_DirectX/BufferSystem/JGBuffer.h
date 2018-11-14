#pragma once
#include"DirectX/JGRCObject.h"
#include"JGBufferEnum.h"
namespace JGRC
{
	/*
	Class : JGBuffer
	@m ComPtr<ID3D11Buffer> m_Buffer     : ����
	@m D3D11_BUFFER_DESC    m_BufferDesc : ���� ���� ���
	@m EJGBufferType        m_BufferType : ���� Ÿ��
	@m EJGBufferState       m_BufferCurrentState : ���� ���� ����
	@m size_t m_BindDataSize : ���ε��� ������ ũ��
	*/
	class DirectX;
	class CORE_EXPORT JGBuffer : public JGRCObject
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
		D3D11_BUFFER_DESC m_BufferDesc;
		EBufferType     m_BufferType;

		size_t m_BindDataSize;
		static DirectX* m_pDirectX;
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
		bool CreateBuffer(EBufferType bufferType, EUsageType usage, ECPUType CPUtype,
			void* data, size_t size);
		/*
		Exp : ������ ����
		@param : EJGMapType type : �� Ÿ��
		@param : void* InData : ��ü�� ������ */
		bool Write(EMapType type, void* InData);

		/*
		Exp : �̱��� */
		bool Read(EMapType type, void* OutData);
		/*
		Exp : ������ �����͸� ����Ѵ�. */
		ID3D11Buffer* Get();
		/*
		Exp : ������ ������ �ּҸ� ����Ѵ�. */
		ID3D11Buffer** GetAddress();
		/*
		Exp : ����Ÿ���� ���´�.*/
		EBufferType GetType();
		/*
		Exp : ���ۼ�������� �����´�. */
		void GetDesc(D3D11_BUFFER_DESC& Desc);
		/*
		Exp : ���ۿ� ���ε��� �������� ����� �����´�. */
		size_t GetBindingDataSize();
	};



}
