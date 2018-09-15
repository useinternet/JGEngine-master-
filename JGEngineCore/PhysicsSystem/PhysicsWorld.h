#pragma once
#include"../EngineStatics/Engine.h"

enum class E2DBodyType;
struct SJG2DBodyDesc;
class JG2DBody;
class JGShape;

//�ӽ�
class JGCollisionCheckManager;
class PhysicsCalculationSystem;
class ENGINE_EXPORT PhysicsWorld
{

private:
	std::vector<std::shared_ptr<JG2DBody>> m_vBodys;

	// ���� ������ �浹 �Ŵ���..(���߿� ���� �и��ؼ�.. ������ �ý��ۿ� ����..)
	std::unique_ptr<JGCollisionCheckManager> m_CollisionManager;
	std::unique_ptr<PhysicsCalculationSystem> m_CaluculSystem;
	float m_Gravity = 0.1f;
public:
	PhysicsWorld();
	~PhysicsWorld();


	JG2DBody* CreateBody(JGShape* shape, const E2DBodyType type);
	float GetGravity();
	std::vector<std::shared_ptr<JG2DBody>>* GetBodyArray();
	void Tick(const float DeltaTime, const float CheckTime);
};