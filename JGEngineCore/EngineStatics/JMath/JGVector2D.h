#pragma once
#include"../Engine.h"
class JGMatrix;



/*
PTMWidth = ScreenWidth / 10;
PTMHeight = ScreenHeight / 10;


Dx, Dy;
Bx, By;
*/
// DirectX -> Box2D ��ǥ
/*
Bx = (Dx - (ScreenWidth / 2)) / PTMWidth;
By = -((Dy - (ScreenHeight / 2) / PTMHeight);
*/
// Box2D -> DirectX ��ǥ
/*
Dx = (Bx * PTMWidth) + (ScreenWidth / 2);
Dy = -((By * PTMHeight) - (ScreenHeight / 2));

*/
// ex ) Dx = 350.0f, Dy = 590.0f;
// Bx = -3.177083333333333;
// By = -0.462962962962963
/*
/*
class : JGVector2D */
class ENGINE_EXPORT JGVector2D
{
private:
	friend JGVector2D ENGINE_EXPORT operator*(JGVector2D& vec, const float scalar);
	friend JGVector2D ENGINE_EXPORT operator*(const float scalar, JGVector2D& vec);
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
	float  Dot(JGVector2D& Vector);
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
	JGVector2D& operator+=(JGVector2D& vec);
	JGVector2D& operator-=(JGVector2D& vec);
	bool operator==(JGVector2D& vec);
	bool operator!=(JGVector2D& vec);

	//
	void TransformCoord(JGMatrix& matrix);
	void TransformNormal(JGMatrix& matrix);
};
// ������ �����ε�
JGVector2D ENGINE_EXPORT operator*(JGVector2D& vec, const float scalar);
JGVector2D ENGINE_EXPORT operator*(const float scalar, JGVector2D& vec);

