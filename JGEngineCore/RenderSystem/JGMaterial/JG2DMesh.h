#pragma once
#include"JGMesh.h"

class JGBufferManager;


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
private:
	float m_Width;
	float m_Height;
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
		const float Width, const float Height,EPivot Pivot = EPivot::TopLeft);
};
