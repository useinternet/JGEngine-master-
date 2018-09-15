#pragma once
#include"../EngineStatics/Engine.h"
class JGVector2D;
class JGShape;
class PhysicsWorld;
class JGCollisionCheck;
class JGCollisionCheckManager;
class PhysicsCalculationSystem;

/*
EnumClass : E2DBodyType
@e Static : ���� ��ü
@e Dyanamic : ���� ��ü */
enum class E2DBodyType
{
	Static,
	Dyanamic
};
/*
Class : JG2DBody 
@m unique_ptr<JGCollisionCheck> m_CollisionChecker : �浹 üĿ
@m JGShape* m_pShape : ��ü�� ���� 
@m PhysicsWorld* m_pWorld : �����ִ� ���� ����
@m 
@m
@m
   */
class ENGINE_EXPORT JG2DBody
{
private:
	friend PhysicsWorld;
	friend JGCollisionCheckManager;
	friend PhysicsCalculationSystem;
private:
	PhysicsWorld * m_pWorld = nullptr;
	std::unique_ptr<JGCollisionCheck> m_CollisionChecker;
	JGShape* m_pShape = nullptr;

	E2DBodyType m_Type;
	std::unique_ptr<JGVector2D> m_Force;
	std::unique_ptr<JGVector2D> m_AccForce;
	
public:
	JG2DBody();
	~JG2DBody();
	/*
	Exp : JGCollisionCheck�� �����. */
	JGCollisionCheck* CreateCollisionChecker();
	/*
	Exp : JGCollisionCheck�� �����´�. */
	JGCollisionCheck* GetCollisionChecker();
	/*
	Exp : ��ü�� ���� ������ �����´�. */
	JGShape* GetShape();
	/*
	Exp : ��ü�� Ÿ���� �����´�. */
	E2DBodyType GetType();
	/*
	Exp : ���� �ӷ��� �����´�. */
	float GetVelocity();
	/*
	Exp : ���� ���ӵ��� �����´�. */
	float GetAcceleration();
	/*
	Exp : ���� ������ ���� �����´�. */
	JGVector2D& GetForce();

protected:
	void CreateBody(PhysicsWorld* world, JGShape* shape, const E2DBodyType type);
	void AddForce(const float x, const float y);
	void AddForce(const JGVector2D& vec);
	void AddForceX(const float x);
	void AddForceY(const float y);
	void SetForce(const float x, const float y);
	void SetForce(const JGVector2D& vec);
	void Tick(const float DeltaTime);
};