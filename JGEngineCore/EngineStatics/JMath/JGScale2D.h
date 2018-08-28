#pragma once
#include"../Engine.h"

class JGScale2D
{
private:
	friend JGScale2D operator*(JGScale2D& vec, const float scalar);
	friend JGScale2D operator*(const float scalar, JGScale2D& vec);
private:
	D3DXVECTOR3 m_Scale2D = { 1.0f,1.0f,1.0f };
public:
	JGScale2D();
	JGScale2D(const float flatNum);
	JGScale2D(const float x, const float y);
	JGScale2D(const JGScale2D& copy);
	~JGScale2D();
	// x,y�� ��ȯ
	float X();
	float Y();
	/*
	Exp : x�� y�� ��� flatNum���� �����Ѵ�. */
	void Set(const float flatNum);
	/*
	Exp : x�� y�� ���� */
	void Set(const float x, const float y);
	/*
	Exp : x�� y�� ���� */
	void Set(const JGScale2D& scale);
	/*
	Exp : �� ������ ���ΰ����� �Ǻ��Ѵ�. */
	bool IsZeroScale();

	// �����ε�
	JGScale2D& operator=(JGScale2D& scale);
	JGScale2D operator+(JGScale2D& scale);
	JGScale2D operator-(JGScale2D& scale);
	bool operator==(JGScale2D& scale);
	bool operator!=(JGScale2D& scale);

};
// �����ε�
JGScale2D operator*(JGScale2D& scale, const float scalar);
JGScale2D operator*(const float scalar, JGScale2D& scale);