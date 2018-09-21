#pragma once
#include"CollisionComponent.h"
#include"../../PhysicsSystem/JGBox2D/JGDynamics/JG2DBody.h"
// �̸� �������� �����ϰ� BeginComponent�Լ��� ������ �ٵ� ����..

// ��ӹ��� Ʈ������ �Լ� ��� ����
// Body ���� �Լ��� �״�� �̿�
// ���� ������ Ű������ְ��ϰ� ų��� Box2D���� ����ϴ� ��ǥ�� �״�� �̿��ϰ� 
// ���ܿ� SetTransForm�� �̿��Ͽ� Box2D��ǥ�� JG��ǥ�� ����
class JGPolygonShape;
class JG2DBody;
enum class E2DBodyType;
class ENGINE_EXPORT Box2DCollisionComponent : public CollisionComponent
{
private:
	std::unique_ptr<JGPolygonShape> m_Shape;
	E2DBodyType m_BodyType;
	JG2DBody*   m_Body = nullptr;
	float m_HalfWidth   = 50.0f;
	float m_HalfHeight  = 50.0f;
	float m_Density     = 1.0f;
	float m_Friction    = 0.3f;
	float m_Restitution = 0.0f;
public:
	Box2DCollisionComponent();
	virtual ~Box2DCollisionComponent();
	virtual void BeginComponent(World* world) override;
	virtual void Tick(const float DeltaTime) override;
	void SetCollisionType(const E2DBodyType type);
	void SetAsBox(const float hw, const float hh);
	void SetDensity(const float density);
	void SetFriction(const float friction);
	void SetRestitution(const float restitution);
	JG2DBody* GetBody();

};