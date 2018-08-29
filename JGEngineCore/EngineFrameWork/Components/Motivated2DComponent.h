#pragma once
#include"MotivatedComponent.h"




class ENGINE_EXPORT Motivated2DComponent : public MotivatedComponent
{
	std::unique_ptr<JGVector2D> m_Location;
	std::unique_ptr<JGAngle2D>  m_Angle;
	std::unique_ptr<JGScale2D>  m_Scale;
	std::unique_ptr<JGMatrix>   m_WorldMatrix;


	std::unique_ptr<JGMatrix>   m_ScaleMatrix;
	std::unique_ptr<JGMatrix>   m_AngleMatrix;
	std::unique_ptr<JGMatrix>   m_TranslationMatrix;
	JGVector2D m_PrevLocation;
	JGAngle2D m_PrevAngle;
	JGScale2D m_PrevScale;
	bool m_bChange = true;
public:
	Motivated2DComponent();
	virtual ~Motivated2DComponent();
	/*
	Exp : ������Ʈ�� �����ǰ� ���� ����Ǵ� �Լ��̴�. ( �θ� Ŭ������ BeginComponent�� ȣ���ؾ��Ѵ�. ) */
	virtual void BeginComponent() override;

	/*
	Exp : �� ������ �ѹ� ����Ǵ� �Լ��̴�. ( �θ� Ŭ������ Tick�� ȣ���ؾ��Ѵ�. ) */
	virtual void Tick(const float DeltaTime) override;
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


protected:
	const JGMatrix& GetWorldMatrix();
};