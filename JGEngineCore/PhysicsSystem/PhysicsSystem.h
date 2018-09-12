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
// �߷� -9.8f;
// ������ = �����׷� * ���� ���
// ���ӵ�
// �ݹ߷� = ź����� * ������ ����

// 
// �Ǻ������� b2Vec2 -> JGVector2D;
// �ִϸ��̼�, ����ƽ



class ENGINE_EXPORT PhysicsSystem
{
private:
	float m_GravityConstant = 0.0f;
public:
	PhysicsSystem();
	~PhysicsSystem();

	void InitPhysicsSystem(const float gravity);
};