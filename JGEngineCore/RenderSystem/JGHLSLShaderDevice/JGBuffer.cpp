#include"JGBuffer.h"
#include"../JGDeviceD.h"
#include"../../EngineStatics/JGLog.h"


JGBuffer::JGBuffer()  {}
JGBuffer::~JGBuffer() {}

bool JGBuffer::CreateBuffer(JGDeviceD* Device, EJGBufferType bufferType, EJGUsageType usage, EJGCPUType CPUtype,
	void* data, size_t size)
{
	m_pDevice = Device;
	m_BufferType = bufferType;
	m_BindDataSize = size;
	HRESULT result = S_OK;
	D3D11_SUBRESOURCE_DATA pSourceData;

	// ���� Ÿ�Կ� ���� ���ε� �÷���
	switch (bufferType)
	{
	case EJGBufferType::ConstantBuffer:
		m_BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		break;
	case EJGBufferType::VertexBuffer:
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		break;
	case EJGBufferType::IndexBuffer:
		m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		break;
	case EJGBufferType::InstanceBuffer:
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		break;
	}
	// �뵵 Ÿ��
	switch (usage)
	{
	case EJGUsageType::Static:
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		break;
	case EJGUsageType::Dynamic:
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		break;
	}

	// CPU �ۼ� Ÿ��
	switch (CPUtype)
	{
	case EJGCPUType::Access_Write:
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case EJGCPUType::Access_Read:
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		break;
	case EJGCPUType::None:
		m_BufferDesc.CPUAccessFlags = 0;
		break;
	}
	m_BufferDesc.ByteWidth = (UINT)size;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	if (bufferType != EJGBufferType::ConstantBuffer)
	{
		// ������ ����
		pSourceData.pSysMem = data;
		pSourceData.SysMemPitch = 0;
		pSourceData.SysMemSlicePitch = 0;
		result = m_pDevice->GetDevice()->CreateBuffer(&m_BufferDesc, &pSourceData, m_Buffer.GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}
	}
	else
	{
		result = m_pDevice->GetDevice()->CreateBuffer(&m_BufferDesc, nullptr , m_Buffer.GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}
	}
	return true;
}

bool JGBuffer::Write(EJGMapType type, void* InData)
{
	// Ÿ�� üũ
	if (type == EJGMapType::Read)
	{
		JGLog::Write(ELogLevel::Warning, TT(
			"The JGMapType of JGBuffer is a read function, but it is a write function."));
		return false;
	}
	// CPU �÷��� üũ
	if (m_BufferDesc.CPUAccessFlags != D3D11_CPU_ACCESS_WRITE)
	{
		JGLog::Write(ELogLevel::Warning, TT(
			"The JGCPUType of JGBuffer is not write function."));
		return false;
	}
	// ȣȯ�� üũ
	if (type == EJGMapType::Write_Discard && m_BufferDesc.Usage != D3D11_USAGE_DYNAMIC)
	{
		JGLog::Write(ELogLevel::Warning, TT(
			"JGMapType is Write_Discard, but the UsageType in the current buffer list is not Dynamic."));
		return false;
	}
	if (type == EJGMapType::Write_NoOverWrite && m_BufferType != EJGBufferType::ConstantBuffer)
	{
		JGLog::Write(ELogLevel::Warning, TT(
			"JGMapType is Write_NoOverWrite, but the EJGBufferType is not ContantBuffer."));
		return false;
	}
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	D3D11_MAP MapFlag;

	switch (type)
	{
	case EJGMapType::Read:
		MapFlag = D3D11_MAP_READ;
		break;
	case EJGMapType::Write:
		MapFlag = D3D11_MAP_WRITE;
		break;
	case EJGMapType::Read_Write:
		MapFlag = D3D11_MAP_READ_WRITE;
		break;
	case EJGMapType::Write_Discard:
		MapFlag = D3D11_MAP_WRITE_DISCARD;
		break;
	case EJGMapType::Write_NoOverWrite:
		MapFlag = D3D11_MAP_WRITE_NO_OVERWRITE;
		break;
	}

	// ���ɰ� ������ ����..
	result = m_pDevice->GetContext()->Map(m_Buffer.Get(), 0, MapFlag, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	void* DataPtr = (void*)mappedResource.pData;
	memcpy(DataPtr, InData, m_BindDataSize);
	// �� ����
	m_pDevice->GetContext()->Unmap(m_Buffer.Get(), 0);

	return true;
}
bool JGBuffer::Read(EJGMapType type, void * OutData)
{
	// �̱���
	return true;
}
ID3D11Buffer* JGBuffer::Get()
{
	if (m_Buffer.Get())
	{
		return m_Buffer.Get();
	}
	JGLog::Write(ELogLevel::Warning, TT(
		"Bubber is not exist."));
	return nullptr;
}
ID3D11Buffer** JGBuffer::GetAddress()
{
	if (m_Buffer.Get())
	{
		return m_Buffer.GetAddressOf();
	}
	JGLog::Write(ELogLevel::Warning, TT(
		"Bubber is not exist."));
	return nullptr;
}
EJGBufferType JGBuffer::GetType()
{
	return m_BufferType;
}
void JGBuffer::GetDesc(D3D11_BUFFER_DESC& Desc)
{
	Desc = m_BufferDesc;
}

size_t JGBuffer::GetBindingDataSize()
{
	return m_BindDataSize;
}
