#pragma once
#include"../EngineStatics/Engine.h"


// ���� ��ȹ
/*

// ���ӵ�, �ӵ� 
���ӵ��� �ӵ��� ��ȭ ��Ű��
�ӵ��� ��ġ�� ��ȭ��Ų��.


F = ma
��ü�� 
( ������ ������. )
����
���ӵ� : ���� / ��

*/




class ENGINE_EXPORT PhysicsSystem
{
private:
	float m_GravityConstant = 0.0f;
public:
	PhysicsSystem();
	~PhysicsSystem();

	void InitPhysicsSystem(const float gravity);
};