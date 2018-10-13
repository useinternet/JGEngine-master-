#pragma once
#include"Component.h"


/*
Class : MotivatedComponent  Ŭ����( ���������� ���� ������Ʈ ) */
class ENGINE_EXPORT MotivatedComponent : public Component
{
public:
	MotivatedComponent();
	virtual ~MotivatedComponent();
	bool m_bDefaultView = false;
protected:
	virtual const JGMatrix& GetViewMatrix();
	const JGMatrix& GetProjectionMatrix();
	const JGMatrix& GetOrthoMatrix();
public:
	void UseDefaultViewMatrix();
};