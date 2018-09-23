#pragma once
#include"Motivated2DComponent.h"
#include"../../PhysicsSystem/JGBox2D/JGDynamics/JG2DBody.h"
#include"../../PhysicsSystem/JGBox2D/JGDynamics/JG2DFilter.h"
// �� ��ü�� ���� �ڱⰡ �������ִ� �ݸ��� ������Ʈ�� ������ ���·� ����..
// �浹 �����Ϳ���  �浹���� �ø��� ������Ʈ�� ������..
// �̵� ���õȰŴ� �ø��� ������Ʈ ���Ŀ� �����Լ��� Box2D �Լ��� ��ü..

class ENGINE_EXPORT CollisionComponent : public Motivated2DComponent
{
private:
	bool m_bBeginOverlap = false;
	bool m_bEndOverlap   = false;
	std::function<void(Object*)> m_BeginOverlapEvent;
	std::function<void(Object*)> m_EndOverlapEvent;
	std::function<void(const std::vector<Object*>&)> m_OverlappingEvent;
	std::unique_ptr<JGVector2D> m_Velocity;
	std::vector<Object*> m_vOverlapObjects;
	Object* m_BeginOverlapObject = nullptr;
	Object* m_EndOverlapObject   = nullptr;


	E2DBodyType m_BodyType;
	JG2DBody*   m_Body = nullptr;
	JG2DFilter  m_BodyFilter;
	float m_Density = 1.0f;
	float m_Friction = 0.3f;
	float m_Restitution = 0.0f;
public:
	CollisionComponent();
	virtual ~CollisionComponent();
	virtual void BeginComponent(World* world) override;
	virtual void Tick(const float DeltaTime) override;




	void SetBeginOverlapEvent(const std::function<void(Object*)>& Event);
	void SetEndOverlapEvent(const std::function<void(Object*)>& Event);
	void SetOverlappingEvent(const std::function<void(const std::vector<Object*>&)>& Event);



	void AddOverlapObject(Object* overlapObject);
	void DeleteOverlapObject(Object* overlapObject);

	E2DBodyType GetBodyType();
	JG2DBody* GetBody();
	float GetDensity();
	float GetFriction();
	float GetRestitution();
	JG2DFilter& GetFilter();
	void SetBodyType(E2DBodyType type);
	void SetBody(JG2DBody* body);
	void SetDensity(const float Density);
	void SetFriction(const float Friction);
	void SetRestitution(const float Restitution);
	void SetFilter(JG2DFilter filter);
public:
	JGVector2D& GetVelocity();
	void SetVelocity(const JGVector2D& vel);


};