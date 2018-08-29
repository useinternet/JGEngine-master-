#pragma once
#include"Component.h"


/*
Class : MotivatedComponent  Ŭ����( ���������� ���� ������Ʈ ) */
class ENGINE_EXPORT MotivatedComponent : public Component
{
private:
	// �� GetWorld()�� �ҷ��� ���� ī�޶��� �� ����� ����ؾ�������..
	// �ϴ� �ӽ÷� ���� ���� ������ ���� ������������...
	std::unique_ptr<JGMatrix> m_ViewMatrix;
public:
	MotivatedComponent();
	virtual ~MotivatedComponent();


protected:
	const JGMatrix& GetViewMatrix();
	const JGMatrix& GetProjectionMatrix();
	const JGMatrix& GetOrthoMatrix();
private:
	void TempViewMatrixInit();

};