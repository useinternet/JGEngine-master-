#pragma once
#include"JGMesh.h"

class JGBufferManager;
class JGFontVertexInformation;

/*
EnumClass : EPivot
@e TopLeft      : ������
@e TopMiddle    : �߰���
@e TopRight     : ��������
@e MiddleLeft   : �����߰�
@e MiddleMiddle : ���
@e MiddleRight  : �������߰�
@e BottomLeft   : ���ʾƷ�
@e BottomMiddle : �߰��Ʒ�
@e BottomRight  : �����ʾƷ� */
enum class ENGINE_EXPORT EPivot
{
	TopLeft      = 0,
	TopMiddle    = 1,
	TopRight     = 2,
	MiddleLeft   = 3,
	MiddleMiddle = 4,
	MiddleRight  = 5,
	BottomLeft   = 6,
	BottomMiddle = 7,
	BottomRight  = 8
};

/*
Class : JG2DMesh */
class ENGINE_EXPORT JG2DMesh : public JGMesh
{
private:
	const UINT m_2DVertexCount = 6;
public:
	typedef struct S2DVertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 tex;
	}S2DVertexType;
	typedef struct SPivotPoint
	{
		float x;
		float y;
	}SPivotPoint;
	typedef struct STexInformaton
	{
		SPivotPoint TopLeft = { 0.0f,0.0f };
		float Width = 1.0f;
		float Height = 1.0f;
		
	}STexInformaton;
private:
	std::vector<S2DVertexType> m_vVertexArray;
	EPivot m_Pivot;
	float m_MeshWidth = 0.0f;
	float m_MeshHeight = 0.0f;
	bool  m_ChangingMesh = false;
public:
	JG2DMesh();
	virtual ~JG2DMesh();
	/*
	Exp : 2D �� �޽��� �����Ѵ�. 
	@param JGBufferManager* BufferManager : ���� �Ŵ��� ������
	@param const wstring& ComponentName  : ������Ʈ �̸�
	@param const float Width  : �ؽ��� ���α���
	@param const float Height : �ؽ��� ���α���
	@param EPivot Pivot : �߽� ��ġ */
	bool Construct2DMesh(JGBufferManager* BufferManager,const std::wstring& ComponentName, 
		const float Width, const float Height,EPivot Pivot = EPivot::TopLeft,const STexInformaton& TexInformation = STexInformaton(),
		const EJGUsageType UsageType = EJGUsageType::Static);
	/*
	Exp : �ؽ�Ʈ�� �޽��� �����Ѵ�. 
	@param JGBufferManager* BufferManager : ���� �Ŵ��� ������
	@param const wstring& ComponentName  : ������Ʈ �̸�
	@param const vector<JGFontVertexInformation>& VertexInformation : ��Ʈ ���� ����
	@param EPivot Pivot : �߽� ��ġ */
	bool ConstructTextMesh(JGBufferManager* BufferManager, const std::wstring& ComponentName,const std::vector<JGFontVertexInformation>& VertexInformation, EPivot Pivot = EPivot::TopLeft);
	/*
	Exp : ���� ������ �����´�. */ //( �̰� �޽÷� �ű��. )
	size_t GetIndexCount();

	/*
	Exp : �޽� Pivot�� �����´�.*/
	EPivot GetPivot();
	/*
	Exp : �޽� ���α��̸� �����´�.*/
	float GetMeshWidth();
	/*
	Exp : �޽� ���α��̸� �����´�.*/
	float GetMeshHeight();
	//
	void MeshReverse(const EReverse reverseType);
	// ������
	virtual void Render(JGDeviceD* Device, ERenderingType RenderingType);
	/*
	Exp : ������*/
	void WriteTextMesh(const std::vector<JGFontVertexInformation>& VertexInformation);
private:
	void CreateVertexArray(const float Width, const float Height, EPivot Pivot, const STexInformaton& TexInformation, float AccX = 0.0f,float AccY = 0.0f);
};
