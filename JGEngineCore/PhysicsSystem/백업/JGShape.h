#pragma once
#include"../../EngineStatics/Engine.h"
class JGVector2D;
class JGAngle2D;
enum class EPivot;

/*
EnumClass : EShapeType 
@e Circle : �� Ÿ��.
@e Rectangle : �簢�� Ÿ�� */
enum class EShapeType
{
	Circle,
	Rectangle
};
/*
Class : JGShape 
@m EShapeType m_ShapeType : ���� Ÿ��
@m unique_ptr<JGVector2D> m_Location :  ��ġ
@m unique_ptr<JGAngle2D> m_Angle : ����. */
class ENGINE_EXPORT JGShape
{
private:
	EShapeType m_ShapeType;
	JGVector2D* m_Location = nullptr;
	JGAngle2D*  m_Angle = nullptr;
public:
	JGShape() = delete;
	JGShape(EShapeType type,JGVector2D* pVector, JGAngle2D* pAngle);
	virtual ~JGShape();
	virtual void Tick();
	EShapeType  GetShapeType();
	JGVector2D& GetLocation();
	JGAngle2D&  GetAngle();

	void AddLocation(JGVector2D& vec);
	void AddAngle(JGAngle2D& angle);

	void SetLocation(const JGVector2D& vec);
	void SetAngle(const JGAngle2D& angle);

	bool IsValue();
};