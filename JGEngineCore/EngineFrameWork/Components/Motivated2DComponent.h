#pragma once
#include"MotivatedComponent.h"

class JGVector2D;
class JGAngle2D;
class JGScale2D;


class Motivated2DComponent : public MotivatedComponent
{
	std::unique_ptr<JGVector2D> m_Location;
	std::unique_ptr<JGAngle2D> m_Angle;
	std::unique_ptr<JGScale2D> m_Scale;

public:
	Motivated2DComponent();
	virtual ~Motivated2DComponent();


	/*
	Exp : ������Ʈ�� ��ġ ���� ���Ѵ�. */
	void AddComponentLocation(const float x, const float y);
	/*
	Exp : ������Ʈ�� ��ġ ���� ���Ѵ�. */
	void AddComponentLocation(JGVector2D& vector);
	/*
	Exp : ������Ʈ�� ���� ���� ���Ѵ�. */
	void AddComponentAngle(const float angle);
	/*
	Exp : ������Ʈ�� ���� ���� ���Ѵ�. */
	void AddComponentAngle(JGAngle2D& angle);
	/*
	Exp : ������Ʈ�� ������ ���� ���Ѵ�. */
	void AddComponentScale(const float flatNum);
	/*
	Exp : ������Ʈ�� ������ ���� ���Ѵ�. */
	void AddComponentScale(const float x, const float y);
	/*
	Exp : ������Ʈ�� ������ ���� ���Ѵ�. */
	void AddComponentScale(JGScale2D& scale);
	/*
	Exp : ������Ʈ ���� ��ġ ���� �����Ѵ�. */
	void SetComponentLocation(const float x, const float y);
	/*
	Exp : ������Ʈ ���� ��ġ ���� �����Ѵ�. */
	void SetComponentLocation(const JGVector2D& vector);
	/*
	Exp : ������Ʈ ���� ���� ���� �����Ѵ�. */
	void SetComponentAngle(const float angle);
	/*
	Exp : ������Ʈ ���� ���� ���� �����Ѵ�. */
	void SetComponentAngle(const JGAngle2D& angle);
	/*
	Exp : ������Ʈ ���� ������ ���� �����Ѵ�. */
	void SetComponentScale(const float flatNum);
	/*
	Exp : ������Ʈ ���� ������ ���� �����Ѵ�. */
	void SetComponentScale(const float x, const float y);
	/*
	Exp : ������Ʈ ���� ������ ���� �����Ѵ�. */
	void SetComponentScale(const JGScale2D& scale);

	/*
	Exp : ������Ʈ ���� ��ġ ���� �����´�. */
	JGVector2D& GetComponentLocalLocation();
	/*
	Exp : ������Ʈ ���� ���� ���� �����´�. */
	JGAngle2D& GetComponentLocalAngle();
	/*
	Exp : ������Ʈ ���� ������ ���� �����´�. */
	JGScale2D& GetComponentLocalScale();
	/*
	Exp : ������Ʈ ���� ��ġ ���� �����´�. */
	JGVector2D GetComponentWorldLocation();
	/*
	Exp : ������Ʈ ���� ���� ���� �����´�. */
	JGAngle2D GetComponentWorldAngle();
	/*
	Exp : ������Ʈ ���� ������ ���� �����´�. */
	JGScale2D GetComponentWorldScale();
};