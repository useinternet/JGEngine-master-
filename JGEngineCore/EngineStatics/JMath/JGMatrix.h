#pragma once
#include"../Engine.h"




class JGVector2D;
class JGAngle2D;
class JGScale2D;
class JGVector3D;
class ENGINE_EXPORT JGMatrix
{
private:
	D3DXMATRIX m_Matrix;

public:
	JGMatrix();
	JGMatrix(const D3DXMATRIX& matrix);
	JGMatrix(const JGMatrix& matrix);
	~JGMatrix();

	/*
	Exp : �� ����� �׵� ��ķ� �����. */
	void Identity();
	/*
	Exp : �� ����� ��ġ ��ķ� �����. */
	void Transpose();
	/*
	Exp : �� ����� ������� ���Ѵ�. 
	@param matrix : �� ����� ������� �� ��� */
	float Inverse(JGMatrix* matrix);
	/*
	Exp : ������ ��ķ� �����. */
	void MakeScalingMatirx(const float x, const float y, const float z);
	/*
	Exp : ������ ��ķ� �����. */
	void MakeScalingMatirx(JGScale2D& Scale);

	/*
	Exp : ȸ�� ����� �����. */
	void MakeRotationMatirx(const float x, const float y, const float z);
	/*
	Exp : ȸ�� ���(2D��)�� �����. */
	void MakeAngle2DMatrix(const float angle);
	/*
	Exp : ȸ�� ���(2D��)�� �����. */
	void MakeAngle2DMatrix(JGAngle2D& angle);
	/*
	Exp : �̵� ����� �����. */
	void MakeTranslationMatrix(const float x, const float y, const float z);
	/*
	Exp : �̵� ���(2D)�� �����. */
	void MakeTranslation2DMatrix(const float ScreenWidth, const float ScreenHeight,const float x, const float y);
	/*
	Exp : �̵� ���(2D)�� �����. */
	void MakeTranslation2DMatrix(const float ScreenWidth, const float ScreenHeight,JGVector2D& vector);

	/*
	Exp : ������� �����. 
	@param JGVector3D location : ��ü�� ��ġ 
	@param JGVector3D lookAt : ��ü�� �ٶ󺸴� ���� 
	@param JGVector3D up : ��ü�� ���� ���� */
	void MakeViewMatrix(JGVector3D* location, JGVector3D* lookAt, JGVector3D* up);

	/*
	Exp : ���� ����� �����.
	@param const float FOV : field of view ( �þ߰� ) 
	@param const float ScreenAspect : ȭ�� ���� 
	@param const float NearZ : �������Ǵ� �ּ� Z�Ÿ� 
	@param const float FarZ : ������ �Ǵ� �ִ� Z�Ÿ� */
	void MakeProjectionMatrix(const float FOV, const float ScreenAspect, const float NearZ, const float FarZ);
	/*
	Exp : ���翵 ����� �����. 
	@param const float width : ��ũ�� ���� ����
	@param const float height : ��ũ�� ���� ����
	@param const float NearZ : �������Ǵ� �ּ� Z�Ÿ�
	@param const float FarZ : ������ �Ǵ� �ִ� Z�Ÿ� */
	void MakeOrthoMatrix(const float width, const float height, const float NearZ, const float FarZ);

	/*
	Exp : �ش� ���Կ� �����ϴ� ���� �����Ѵ�. ( 4x4 ����̴�. )*/
	void SetElement(const UINT slot, const float Value);
	/*
	Exp : �ش� ���Կ� �����ϴ� ���� �����Ѵ�. ( 4x4 ����̴�. )*/
	float GetElement(const UINT slot);

	/*
	Exp : ������ ���� ������ �����´�. ( 4x4 ����̴�. )*/
	D3DXMATRIX& Get();

	/*
	Exp : ������ ���� �� �ּҷ� �����´�. ( 4x4 ����̴�. )*/
	D3DXMATRIX* GetAddress();

	// ������ �����ε�
	JGMatrix& operator=(const JGMatrix& matrix);

	JGMatrix operator+(JGMatrix& matrix);
	JGMatrix operator-(JGMatrix& matrix);
	JGMatrix operator*(JGMatrix& matrix);

	JGMatrix& operator+=(JGMatrix& matrix);
	JGMatrix& operator-=(JGMatrix& matrix);
	JGMatrix& operator*=(JGMatrix& matrix);
	
};