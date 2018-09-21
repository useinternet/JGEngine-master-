#pragma once
#include"Motivated2DComponent.h"


// �� ��ü�� ���� �ڱⰡ �������ִ� �ݸ��� ������Ʈ�� ������ ���·� ����..
// �浹 �����Ϳ���  �浹���� �ø��� ������Ʈ�� ������..
// �̵� ���õȰŴ� �ø��� ������Ʈ ���Ŀ� �����Լ��� Box2D �Լ��� ��ü..

//
class ENGINE_EXPORT CollisionComponent : public Motivated2DComponent
{
private:
	bool m_bBeginOverlap = false;
	bool m_bEndOverlap   = false;
	std::unique_ptr<JGVector2D> m_Velocity;
	std::vector<Object*> m_vOverlapObjects;
	Object* m_BeginOverlapObject = nullptr;
	Object* m_EndOverlapObject   = nullptr;
public:
	CollisionComponent();
	virtual ~CollisionComponent();
	virtual void Tick(const float DeltaTime) override;
	virtual void BeginOverlap(Object* OverlapObject);
	virtual void Overlapping(std::vector<Object*>& OverlapObjectArray);
	virtual void EndOverlap(Object* OverlapObject);
	void AddOverlapObject(Object* overlapObject);
	void DeleteOverlapObject(Object* overlapObject);
public:
	JGVector2D& GetVelocity();
	void SetVelocity(const JGVector2D& vel);


};