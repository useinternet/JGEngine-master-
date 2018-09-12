#pragma once
#include"../EngineStatics/Engine.h"
class JGShape;
class PhysicsWorld;
/*
struct : SJG2DBodyDesc 
@m float Mass : ����
@m float Friction : ������
@m float Repulsive : �ݹ߷� */
typedef struct SJG2DBodyDesc
{
	float Mass      = 0.0f;
	float Friction  = 0.0f;
	float Repulsive = 0.0f;
}JGBodyDesc;
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
Class : JG2DBody */
class ENGINE_EXPORT JG2DBody
{
private:
	JGShape* m_pShape = nullptr;
	PhysicsWorld* m_pWorld = nullptr;
	E2DBodyType m_Type;
	
	std::unique_ptr<SJG2DBodyDesc> m_BodyDesc;
	float m_Velocity = 0.0f;
	float m_Acceleration = 0.0f;
	
public:
	JG2DBody();
	~JG2DBody();
	void CreateBody(PhysicsWorld* world,JGShape* shape, const E2DBodyType type,const SJG2DBodyDesc& BodyDesc);

	void Tick(const float DeltaTime);
	JGShape* GetShape();
	E2DBodyType GetType();
	float GetVelocity();
	float GetAcceleration();
};