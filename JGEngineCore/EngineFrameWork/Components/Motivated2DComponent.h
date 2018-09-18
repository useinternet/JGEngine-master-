#pragma once
#include"MotivatedComponent.h"



/*
Class : Motivated2DComponent 
Exp : ���������� �������ִ� 2D ������Ʈ 
@m unique_ptr<JGVector2D> m_Location : ��ġ ��ǥ
@m unique_ptr<JGAngle2D> m_Angle     : ���� 
@m unique_ptr<JGScale2D> m_Scale     : ������ 
@m unique_ptr<JGMatrix>  m_WorldMatrix : ���� �� ��Ÿ�� ��(��ġ,����,������)�� �����ϱ����� ��� 
@m unique_ptr<JGMatrix>  m_ScaleMatrix : ������ ���� ��Ÿ�� ��� 
@m unique_ptr<JGMatrix>  m_AngleMatrix : ���� ���� ��Ÿ�� ��� 
@m unique_ptr<JGMatrix>  m_TranslationMatrix : �̵� ���� ��Ÿ�� ��� 
@m JGVector2D m_PrevLocation : ��ġ ��ǥ ���� ��
@m JGAngle2D m_PrevAngle  : ���� ���� ��
@m JGScale2D m_PrevScale : ������ ���� ��
@m m_bChange : ��ġ, ����, �������� �ϳ��� �ٲ������ ���� (Rendering��)
@m m_bIsChangeLocation : ��ġ ���� ���� 
@m m_bIsChangeAngle : ���� ���� ���� 
@m m_bIsChangeScale : ������ ���� ���� */
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
	bool m_bIsChangeLocation = false;
	bool m_bIsChangeAngle    = false;
	bool m_bIsChangeScale    = false;
public:
	Motivated2DComponent();
	virtual ~Motivated2DComponent();
	/*
	Exp : ������Ʈ�� �����ǰ� ���� ����Ǵ� �Լ��̴�. ( �θ� Ŭ������ BeginComponent�� ȣ���ؾ��Ѵ�. ) */
	virtual void BeginComponent(World* world) override;

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
	Exp : ������Ʈ ���� ��ġ ���� �����´�. */
	JGVector2D GetComponentWorldLocation();
	/*
	Exp : ������Ʈ ���� ���� ���� �����´�. */
	JGAngle2D GetComponentWorldAngle();
	/*
	Exp : ������Ʈ ������ ���� �����´�. */
	JGScale2D& GetComponentScale();
	
	/*
	Exp : ��ġ ���� �ٲ������ true�� �״���̸� false�� ��ȯ�Ѵ�. */
	bool IsChangeLocation();
	/*
	Exp : ���� ���� �ٲ������ true�� �״���̸� false�� ��ȯ�Ѵ�. */
	bool IsChangeAngle();
	/*
	Exp : ������ ���� �ٲ������ true�� �״�b���̸� false�� ��ȯ�Ѵ�. */
	bool IsChangeScale();

	/*
	Exp :*/
	JGVector2D* GetComponentLocationAddress();
	JGAngle2D*  GetComponentAngleAddress();

protected:
	/*
	Exp : �� ������Ʈ�� ���� ����� �����´�. */
	const JGMatrix& GetWorldMatrix();

private:
	virtual bool Render2DCurling();
};