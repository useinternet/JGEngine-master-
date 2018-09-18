#pragma once
#include"Component.h"


/*
Class : MotivatedComponent  Ŭ����( ���������� ���� ������Ʈ ) */
class ENGINE_EXPORT MotivatedComponent : public Component
{
public:
	MotivatedComponent();
	virtual ~MotivatedComponent();


protected:
	const JGMatrix& GetViewMatrix();
	const JGMatrix& GetProjectionMatrix();
	const JGMatrix& GetOrthoMatrix();


};