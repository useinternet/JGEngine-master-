#include"Motivated2DComponent.h"
#include"../../RenderSystem/JGViewportD.h"
using namespace std;

Motivated2DComponent::Motivated2DComponent()
{
	RegisterComponentID(typeid(this));
	m_Location = make_unique<JGVector2D>();
	m_Angle = make_unique<JGAngle2D>();
	m_Scale = make_unique<JGScale2D>();
	m_WorldMatrix = make_unique<JGMatrix>();
	m_ScaleMatrix = make_unique<JGMatrix>();
	m_AngleMatrix = make_unique<JGMatrix>();
	m_TranslationMatrix = make_unique<JGMatrix>();
}
Motivated2DComponent::~Motivated2DComponent()
{
	
}
void Motivated2DComponent::BeginComponent()
{
	MotivatedComponent::BeginComponent();
	// ��� �ʱ�ȭ
	m_WorldMatrix->Identity();
	m_ScaleMatrix->MakeScalingMatirx(*m_Scale);
	m_AngleMatrix->MakeAngle2DMatrix(*m_Angle);
	m_TranslationMatrix->MakeTranslation2DMatrix(
		GetViewport()->GetWidth(), GetViewport()->GetHeight(),
		*m_Location);
}
void Motivated2DComponent::Tick(const float DeltaTime)
{
	MotivatedComponent::Tick(DeltaTime);

	// ������, ����, ��ġ���� ������ ����� �������� �ʴ´�.
	if (*m_Scale != m_PrevScale)
	{
		m_ScaleMatrix->MakeScalingMatirx(*m_Scale);
		m_PrevScale  = *m_Scale;
		m_bChange = true;
	}
	if (*m_Angle != m_PrevAngle)
	{
		m_AngleMatrix->MakeAngle2DMatrix(*m_Angle);
		m_PrevAngle = *m_Angle;
		m_bChange = true;
	}
	if (*m_Location != m_PrevLocation)
	{
		m_TranslationMatrix->MakeTranslation2DMatrix(
			GetViewport()->GetWidth(), GetViewport()->GetHeight(), 
			*m_Location);
		m_PrevLocation = *m_Location;
		m_bChange = true;
	}
	if (m_bChange)
	{
		m_bChange = false;
		*m_WorldMatrix = (*m_ScaleMatrix) * (*m_AngleMatrix) * (*m_TranslationMatrix);
	}
}
void Motivated2DComponent::AddComponentLocation(const float x, const float y)
{
	m_Location->Set(m_Location->X() + x, m_Location->Y() + y);
}
void Motivated2DComponent::AddComponentLocation(JGVector2D& vector)
{
	m_Location->Set(m_Location->X() + vector.X(),
		m_Location->Y() + vector.Y());
}
void Motivated2DComponent::AddComponentAngle(const float angle)
{
	m_Angle->Set(m_Angle->Get() + angle);
}
void Motivated2DComponent::AddComponentAngle(JGAngle2D& angle)
{
	m_Angle->Set(m_Angle->Get() + angle);
}
void Motivated2DComponent::AddComponentScale(const float flatNum)
{
	m_Scale->Set(m_Scale->X() + flatNum, m_Scale->Y() + flatNum);
}
void Motivated2DComponent::AddComponentScale(const float x, const float y)
{
	m_Scale->Set(m_Scale->X() + x, m_Scale->Y() + y);
}
void Motivated2DComponent::AddComponentScale(JGScale2D& scale)
{
	m_Scale->Set(m_Scale->X() + scale.X(), m_Scale->Y() + scale.Y());
}
void Motivated2DComponent::SetComponentLocation(const float x, const float y)
{
	m_Location->Set(x, y);
}
void Motivated2DComponent::SetComponentLocation(const JGVector2D& vector)
{
	m_Location->Set(vector);
}
void Motivated2DComponent::SetComponentAngle(const float angle)
{
	m_Angle->Set(angle);
}
void Motivated2DComponent::SetComponentAngle(const JGAngle2D& angle)
{
	m_Angle->Set(angle);
}
void Motivated2DComponent::SetComponentScale(const float flatNum)
{
	m_Scale->Set(flatNum);
}
void Motivated2DComponent::SetComponentScale(const float x, const float y)
{
	m_Scale->Set(x, y);
}
void Motivated2DComponent::SetComponentScale(const JGScale2D& scale)
{
	m_Scale->Set(scale);
}


JGVector2D& Motivated2DComponent::GetComponentLocalLocation()
{
	return *m_Location;
}
JGAngle2D& Motivated2DComponent::GetComponentLocalAngle()
{
	return *m_Angle;
}
JGScale2D& Motivated2DComponent::GetComponentLocalScale()
{
	return *m_Scale;
}
JGVector2D Motivated2DComponent::GetComponentWorldLocation()
{
	Component* ParentComp = GetParent();
	if (ParentComp)
	{
		Motivated2DComponent*  ParentM2DComp = dynamic_cast<Motivated2DComponent*>(ParentComp);
		if (ParentM2DComp)
		{
			return ParentM2DComp->GetComponentWorldLocation() + (*m_Location);
		}
	}
	return *m_Location;
}
JGAngle2D Motivated2DComponent::GetComponentWorldAngle()
{
	Component* ParentComp = GetParent();
	if (ParentComp)
	{
		Motivated2DComponent*  ParentM2DComp = dynamic_cast<Motivated2DComponent*>(ParentComp);
		if (ParentM2DComp)
		{
			return ParentM2DComp->GetComponentWorldAngle() + (*m_Angle);
		}
	}
	return *m_Angle;
}
JGScale2D Motivated2DComponent::GetComponentWorldScale()
{
	Component* ParentComp = GetParent();
	if (ParentComp)
	{
		Motivated2DComponent*  ParentM2DComp = dynamic_cast<Motivated2DComponent*>(ParentComp);
		if (ParentM2DComp)
		{
			return ParentM2DComp->GetComponentWorldScale() + (*m_Scale);
		}
	}
	return *m_Scale;
}

const JGMatrix& Motivated2DComponent::GetWorldMatrix()
{
	return *m_WorldMatrix;
}


