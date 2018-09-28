#pragma once
#include"CollisionComponent.h"

// �̸� �������� �����ϰ� BeginComponent�Լ��� ������ �ٵ� ����..

// ��ӹ��� Ʈ������ �Լ� ��� ����
// Body ���� �Լ��� �״�� �̿�
// ���� ������ Ű������ְ��ϰ� ų��� Box2D���� ����ϴ� ��ǥ�� �״�� �̿��ϰ� 
// ���ܿ� SetTransForm�� �̿��Ͽ� Box2D��ǥ�� JG��ǥ�� ����
class JGPolygonShape;
enum class EReSizePivot
{
	Up,
	Down,
	Middle
};
class ENGINE_EXPORT Box2DCollisionComponent : public CollisionComponent
{
private:
	std::unique_ptr<JGPolygonShape> m_Shape;
	float m_HalfWidth   = 50.0f;
	float m_HalfHeight  = 50.0f;
	bool  m_bInit = false;
public:
	Box2DCollisionComponent();
	virtual ~Box2DCollisionComponent();
	virtual void BeginComponent(World* world) override;
	void SetAsBox(const float hw, const float hh, EReSizePivot Pivot = EReSizePivot::Down);
};