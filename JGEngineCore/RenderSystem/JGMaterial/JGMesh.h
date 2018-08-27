#pragma once
#include"../../EngineStatics/Engine.h"


class JGBufferManager;
class JGDeviceD;
enum class EJGBufferType;
enum class EJGUsageType;
enum class EJGCPUType;
enum class EJGMapType;


class ENGINE_EXPORT JGMesh
{
private:
	std::vector<UINT> m_Stride;
	std::vector<UINT> m_Offset;
	std::vector<ID3D11Buffer*> m_VertexBuffer;
	std::vector<ID3D11Buffer*> m_IndexBuffer;
	JGBufferManager* m_BufferManager = nullptr;
	bool bUseInstanceBuffer = false;
public:
	JGMesh(const JGMesh& copy) = delete;
	JGMesh();
	~JGMesh();

	/*
	Exp : ���� / �ε��� ���� ���� */
	bool Create_Vertex_Index_Buffer(JGBufferManager* BufferManager,const std::wstring& MeshName,
		EJGUsageType VertexUsageType, EJGCPUType VertexCPUType, EJGUsageType IndexUsageType, EJGCPUType IndexCPUType,
		void* VertexData,size_t DataCount, size_t DataTypeSize);

	/*
	Exp : �̱��� (���� ������ ��)*/
	bool Create_Vertex_Instance_Buffer(JGBufferManager* BufferManager);

	void Render(JGDeviceD* Device, D3D_PRIMITIVE_TOPOLOGY RenderingType);
};