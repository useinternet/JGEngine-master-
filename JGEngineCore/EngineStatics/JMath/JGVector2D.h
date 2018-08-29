#pragma once
#include"../Engine.h"
class JGMatrix;
/*
class : JGVector2D */
class ENGINE_EXPORT JGVector2D
{
private:
	friend JGVector2D operator*(JGVector2D& vec, const float scalar);
	friend JGVector2D operator*(const float scalar, JGVector2D& vec);
private:
	D3DXVECTOR3 m_Vector2D = { 0.0f,0.0f,0.0f };

public:
	JGVector2D();
	JGVector2D(const float x, const float y);
	JGVector2D(const JGVector2D& copy);
	~JGVector2D();

	/*
	Exp : �� ���Ͱ� ���κ������� �Ǻ��Ѵ�. */
	bool IsZeroVector();
	/*
	Exp : ������ ���Ѵ�. */
	JGVector2D  Dot(JGVector2D& Vector);
	/*
	Exp : �� ���͸� ����ȭ�Ѵ�.*/
	void  Normalize();
	/*
	Exp : �� ���͸� ���̸� ���Ѵ�.*/
	float Length();
	/*
	Exp : �� ���͸� ���̸� ������ ���� �״�� ���Ѵ�.*/
	float LengthSquard();
	// x,y�� ��ȯ
	float X();
	float Y();
	/*
	Exp : �� ���Ͱ� ����.*/
	void Set(const float x, const float y);
	/*
	Exp : �� ���Ͱ� ����.*/
	void Set(const JGVector2D& vec);

	// ������ �����ε�
	JGVector2D& operator=(JGVector2D& vec);
	JGVector2D operator+(JGVector2D& vec);
	JGVector2D operator-(JGVector2D& vec);
	bool operator==(JGVector2D& vec);
	bool operator!=(JGVector2D& vec);

	//
	void TransformCoord(JGMatrix& matrix);
	void TransformNormal(JGMatrix& matrix);
};
// ������ �����ε�
JGVector2D operator*(JGVector2D& vec, const float scalar);
JGVector2D operator*(const float scalar, JGVector2D& vec);

