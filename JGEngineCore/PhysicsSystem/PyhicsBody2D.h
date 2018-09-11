#pragma once
#include"../EngineStatics/Engine.h"
#include"PhysicsBodyType.h"
// F = ma;
// �߷� -9.8f;
// ������ = �����׷� * ���� ���
// ���ӵ�
// �ݹ߷� = ź����� * ������ ����

// 
// �Ǻ������� b2Vec2 -> JGVector2D;
// �ִϸ��̼�, ����ƽ

class JGVector2D;
class JGAngle2D;
enum class EPivot;
enum class EPyhicsType
{
	Dynamic,
	Static,
	Kinematic
};
enum class EBodyType
{
	Circle,
	Box,
	Capsule
};
enum class EPyhicsBodyState
{
	Active,
	Behind,
	Destroy
};
typedef struct SBodyComposition
{
	float Mass;
	float Friction;
	float Repulsive;
}SBodyComposition;
class ENGINE_EXPORT PyhicsBody2D
{
private:
	EBodyType m_BodyType;
	EPyhicsType m_PyhicsType;
	JGVector2D* m_pLinkingLocation;
	JGAngle2D*  m_pLinkingAngle;
	EPivot m_Pivot;
	EPyhicsBodyState m_BodyState;
	std::unique_ptr<Figure> m_Figure;
	std::unique_ptr<SBodyComposition> m_Composition;
public:
	PyhicsBody2D();
	~PyhicsBody2D();
	void CreateBody(const EBodyType bodytype,const EPyhicsType pyhicstype,const SBodyComposition& Composition,
		const EPivot pivot,JGVector2D* pLocation, JGAngle2D* pAngle);
	/*
	Exp : �ڽ� ����. */
	void SetBox(const float width,const float height);
	/*
	Exp : �̱��� */
	void SetCircle();
	/*
	Exp : �̱��� */
	void SetCapsule();

	void Tick(const float DeltaTime,const float Gravity, std::vector<std::shared_ptr<PyhicsBody2D>>* bodyArray);
private:

	// �ϴ� �������� �غ���
	bool BoxToBoxCollisionCheck(Box* box1,Box* box2);
	bool BoxToPointCollisionCheck(Box* box, JGVector2D& point);
	bool BoxToCircleCollisionCheck(Box* box, Circle* circle);
	bool CircleToPointCollisionCheck(Circle* circle,JGVector2D& point);
	/*
	Exp : ���� ���� ��ü Ÿ���� �����´�. */
	EBodyType GetBodyType();
	/*
	Exp : ���� ���� ��ü ��Ȳ�� �����´�. */
	EPyhicsBodyState GetBodyState();
};


