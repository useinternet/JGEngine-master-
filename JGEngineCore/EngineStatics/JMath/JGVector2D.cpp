#include "JGVector2D.h"
#include"JGMath.h"
#include"JGMatrix.h"
#include"../../RenderSystem/JGViewportD.h"
JGVector2D::JGVector2D()
{
}

JGVector2D::JGVector2D(const float x, const float y)
{
	m_Vector2D.x = x;
	m_Vector2D.y = y;
}

JGVector2D::JGVector2D(const JGVector2D & copy)
{
	m_Vector2D = copy.m_Vector2D;
}

JGVector2D::JGVector2D(const b2Vec2& copy)
{
	m_Vector2D.x = (copy.x * JGViewportD::GetPTM_Width()) + (JGViewportD::GetWidth() / 2);
	m_Vector2D.y = -((copy.y * JGViewportD::GetPTM_Height()) - (JGViewportD::GetHeight() / 2));
}

JGVector2D::~JGVector2D()
{
}


bool JGVector2D::IsZeroVector()
{
	if (JGMath::IsZero(m_Vector2D.x) && JGMath::IsZero(m_Vector2D.y))
	{
		return true;
	}
	return false;
}
float  JGVector2D::Dot(JGVector2D& Vector)
{
	return (Vector.m_Vector2D.x * m_Vector2D.x) +  (Vector.m_Vector2D.y * m_Vector2D.y);
}
void JGVector2D::Normalize()
{
	float lengthSqrd = LengthSquard();
	if (JGMath::IsZero(lengthSqrd))
	{
		m_Vector2D = { 0.0f,0.0f,0.0f };
		return;
	}

	float InverselengthSqrd = 1 / Length();
	m_Vector2D.x *= InverselengthSqrd;
	m_Vector2D.y *= InverselengthSqrd;
}
float JGVector2D::Length()
{
	return JGMath::Sqrt(LengthSquard());
}
float JGVector2D::LengthSquard()
{
	return (m_Vector2D.x * m_Vector2D.x) + (m_Vector2D.y * m_Vector2D.y);
}

float JGVector2D::X()
{
	return m_Vector2D.x;
}

float JGVector2D::Y()
{
	return m_Vector2D.y;
}
b2Vec2 JGVector2D::GetBox2DVec()
{
	return b2Vec2(
		(m_Vector2D.x - (JGViewportD::GetWidth() / 2)) / JGViewportD::GetPTM_Width(),
		-((m_Vector2D.y - (JGViewportD::GetHeight() / 2)) / JGViewportD::GetPTM_Height()));
}
void JGVector2D::SetBox2DVec(const b2Vec2& vec)
{
	m_Vector2D.x = (vec.x * JGViewportD::GetPTM_Width()) + (JGViewportD::GetWidth() / 2);
	m_Vector2D.y = -((vec.y * JGViewportD::GetPTM_Height()) - (JGViewportD::GetHeight() / 2));
}
void JGVector2D::Set(const float x, const float y)
{
	m_Vector2D.x = x;
	m_Vector2D.y = y;
}
void JGVector2D::Set(const JGVector2D & vec)
{
	m_Vector2D.x = vec.m_Vector2D.x;
	m_Vector2D.y = vec.m_Vector2D.y;
}
JGVector2D& JGVector2D::operator=(JGVector2D& vec)
{
	m_Vector2D = vec.m_Vector2D;
	return *this;
}
JGVector2D JGVector2D::operator+(JGVector2D& vec)
{
	return JGVector2D(m_Vector2D.x + vec.m_Vector2D.x,
		m_Vector2D.y + vec.m_Vector2D.y);
	
}
JGVector2D JGVector2D::operator-(JGVector2D& vec)
{
	return JGVector2D(m_Vector2D.x - vec.m_Vector2D.x,
		m_Vector2D.y - vec.m_Vector2D.y);
}
JGVector2D& JGVector2D::operator+=(JGVector2D& vec)
{
	m_Vector2D.x += vec.X();
	m_Vector2D.y += vec.Y();
	return *this;
}
JGVector2D& JGVector2D::operator-=(JGVector2D& vec)
{
	m_Vector2D.x -= vec.X();
	m_Vector2D.y -= vec.Y();
	return *this;
}
bool JGVector2D::operator==(JGVector2D & vec)
{
	if (IsZeroVector() && vec.IsZeroVector())
	{
		return true;
	}
	if (JGMath::Equal(m_Vector2D.x,vec.m_Vector2D.x ) &&
		JGMath::Equal(m_Vector2D.y ,vec.m_Vector2D.y))
	{
		return true;
	}
	return false;
}

bool JGVector2D::operator!=(JGVector2D & vec)
{
	bool result = this->operator==(vec);
	return !result;
}
void JGVector2D::TransformCoord(JGMatrix& matrix)
{
	D3DXVec3TransformCoord(&m_Vector2D, &m_Vector2D, matrix.GetAddress());
}
void JGVector2D::TransformNormal(JGMatrix& matrix)
{
	D3DXVec3TransformNormal(&m_Vector2D, &m_Vector2D, matrix.GetAddress());
}
JGVector2D operator*(JGVector2D & vec, const float scalar)
{
	return JGVector2D(vec.m_Vector2D.x * scalar,vec.m_Vector2D.y * scalar);
}

JGVector2D operator*(const float scalar, JGVector2D & vec)
{
	return JGVector2D(vec.m_Vector2D.x * scalar, vec.m_Vector2D.y * scalar);
}