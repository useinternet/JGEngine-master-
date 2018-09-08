#pragma once
#include"../EngineStatics/Engine.h"


// ���� ��ȹ
/*
b2world (world�� �ϳ�)        


// ���� ���� ��� 
1. b2World ����( �߷°� )
2. b2BodyDef ����( �ٵ� �� ����)
3. b2Body* ������ = b2World::CreateBody�� ����
4. b2PolygonShape�� ���� ���� Ŭ������ CreateFixture�� ����;

// b2FixtureDef �� ���̳��� ������Ʈ ���� ����
// b2FixtureŬ������ �̰͵� CreateFixture�� ����.. 

*/




class ENGINE_EXPORT PhysicsSystem
{
private:
	std::map<const std::wstring, std::unique_ptr<b2World>> m_mPhysicsWorlds;
	std::unique_ptr<b2World> m_PhysicsWorld;


public:
	PhysicsSystem();
	~PhysicsSystem();

	void InitPhysicsSystem(const float gravity);
};