#pragma once
#include"../Engine.h"

class JGMatrix;
class ENGINE_EXPORT JGVector3D
{
private:
	friend JGVector3D operator*(JGVector3D& vec, const float scalar);
	friend JGVector3D operator*(const float scalar, JGVector3D& vec);
private:
	D3DXVECTOR3 m_Vector3D = { 0.0f,0.0f,0.0f };

public:
	JGVector3D();
	JGVector3D(const float x, const float y, const float z);
	JGVector3D(const JGVector3D& copy);
	~JGVector3D();

	/*
	Exp : �� ���Ͱ� ���κ������� �Ǻ��Ѵ�. */
	bool IsZeroVector();
	/*
	Exp : ������ ���Ѵ�. */
	JGVector3D  Dot(JGVector3D& Vector);
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
	float Z();
	/*
	Exp : �� ���Ͱ� ����.*/
	void Set(const float x, const float y,const float z);
	/*
	Exp : �� ���Ͱ� ����.*/
	void Set(const JGVector3D& vec);


	// ��� �����
	D3DXVECTOR3& Get();
	D3DXVECTOR3* GetAddress();
	// ������ �����ε�
	JGVector3D& operator=(JGVector3D& vec);
	JGVector3D operator+(JGVector3D& vec);
	JGVector3D operator-(JGVector3D& vec);
	bool operator==(JGVector3D& vec);
	bool operator!=(JGVector3D& vec);
	/*
	Exp : ��Ŀ� w���� 1�� �����Ͽ� ����, �̵����� ����... */
	void TransformCoord(JGMatrix& matrix);
	/*
	Exp : ��Ŀ� w���� 0���� �����Ͽ� ����, �̵����� ����.*/
	void TransformNormal(JGMatrix& matrix);
};

// ������ �����ε�
JGVector3D operator*(JGVector3D& vec, const float scalar);
JGVector3D operator*(const float scalar, JGVector3D& vec);