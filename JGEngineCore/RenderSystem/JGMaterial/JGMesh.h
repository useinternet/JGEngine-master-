#pragma once
#include"../../EngineStatics/Engine.h"


class JGBufferManager;
class JGDeviceD;
enum class EJGBufferType;
enum class EJGUsageType;
enum class EJGCPUType;
enum class EJGMapType;

/*
EnumClass : ERenderingType 
@e PointList : �� ��� Ÿ��
@e LineList : �� ��� Ÿ��
@e LineStrip : �� ���� ��Ʈ�� Ÿ�� 
@e TriangleList : �ﰢ�� ��� Ÿ��
@e TriangleStrip : �ﰢ�� ��Ʈ�� */
enum class ENGINE_EXPORT ERenderingType
{
	PointList = 0,
	LineList = 1,
	LineStrip = 2,
	TriangleList = 3,
	TriangleStrip = 4
};
/*
Class : JGMesh
@m vector<UINT> m_Stride : ������ ����� �迭
@m vector<UINT> m_Offset : ������ ��ġ �� �迭
@m vector<ID3D11Buffer*> m_VertexBuffer : ���� ���� ������ �迭
@m vector<ID3D11Buffer*> m_IndexBuffer : �ε��� ���� ������ �迭
@m JGBufferManager* m_BufferManager : ���� �Ŵ��� ������
@m bUseInstanceBuffer : �ν��Ͻ��� ����(�̱���) */
class ENGINE_EXPORT JGMesh
{
private:
	std::vector<UINT> m_Stride;
	std::vector<UINT> m_Offset;
	std::vector<ID3D11Buffer*> m_VertexBuffer;
	std::vector<ID3D11Buffer*> m_IndexBuffer;
	JGBufferManager* m_BufferManager = nullptr;
	bool bUseInstanceBuffer          = false;
public:
	JGMesh(const JGMesh& copy) = delete;
	JGMesh();
	virtual ~JGMesh();

	/*
	Exp : ���� / �ε��� ���� ���� 
	@param JGBufferManager* BufferManager   : ���� �Ŵ��� 
	@param const wstring    MeshName        : �޽� �̸�
	@param EJGUsageType     VertexUsageType : ���� �뵵 Ÿ��
	@param EJCPUType        VertexCPUType   : ���� CPU Ÿ��
	@param EJGUsageType     IndexUsageType  : �ε��� �뵵 Ÿ�� 
	@param EJGCPUType       IndexCPUType    : �ε��� CPU Ÿ��
	@param void*  VertexData   : ���� ������
	@param size_t DataCount    : ������ ����
	@param size_t DataTypeSize : ������ Ÿ�� ������*/
	bool Create_Vertex_Index_Buffer(JGBufferManager* BufferManager,const std::wstring& MeshName,
		EJGUsageType VertexUsageType, EJGCPUType VertexCPUType, EJGUsageType IndexUsageType, EJGCPUType IndexCPUType,
		void* VertexData,size_t DataCount, size_t DataTypeSize);

	/*
	Exp : �̱��� (���� ������ ��)*/
	bool Create_Vertex_Instance_Buffer(JGBufferManager* BufferManager);

	void Render(JGDeviceD* Device, ERenderingType RenderingType);
};