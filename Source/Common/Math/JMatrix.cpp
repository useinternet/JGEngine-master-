#include "pch.h"
#include "JMatrix.h"
#include "JVector.h"
#include "JQuaternion.h"

JMatrix::JMatrix() :
	m_Data(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f) {}
JMatrix::JMatrix(float init_value) : 
	m_Data(
		init_value, init_value, init_value, init_value,
		init_value, init_value, init_value, init_value,
		init_value, init_value, init_value, init_value,
		init_value, init_value, init_value, init_value) {}

JMatrix::JMatrix(
	float w,
	const JVector3& v1,
	const JVector3& v2,
	const JVector3& v3,
	const JVector3& v4) :
	m_Data(
		v1.x, v1.y, v1.z, w,
		v2.x, v2.y, v2.z, w,
		v3.x, v3.y, v3.z, w,
		v4.x, v4.y, v4.z, w) {};


JMatrix::JMatrix(
	const JVector3& v1, float w1,
	const JVector3& v2, float w2,
	const JVector3& v3, float w3,
	const JVector3& v4, float w4) :
	m_Data(
		v1.x, v1.y, v1.z, w1,
		v2.x, v2.y, v2.z, w2,
		v3.x, v3.y, v3.z, w3,
		v4.x, v4.y, v4.z, w4) { }

JVector3 JMatrix::TransformPoint(const JVector3& v)
{
	auto sim_v = DirectX::XMVector3TransformCoord(JVector3::GetSIMD(v), GetSIMD());
	return JVector3::ConvertJVector3(sim_v);
}
JVector3 JMatrix::TransformVector(const JVector3& v)
{
	auto sim_v = DirectX::XMVector3TransformNormal(JVector3::GetSIMD(v), GetSIMD());
	return JVector3::ConvertJVector3(sim_v);
}
JMatrix JMatrix::Translation(const JVector3& v)
{
	JMatrix m;
	m.SetSIMD(DirectX::XMMatrixTranslation(v.x, v.y, v.z));
	return m;
}
JMatrix JMatrix::Rotation(const JVector3& v)
{
	JMatrix m;
	m.SetSIMD(DirectX::XMMatrixRotationRollPitchYaw(v.x, v.y, v.z));
	return m;
}

JMatrix JMatrix::Rotation(const JQuaternion& q)
{
	JMatrix m;
	m.SetSIMD(DirectX::XMMatrixRotationQuaternion(q.GetSIMD()));
	return m;
}
JMatrix JMatrix::RotationAxis(const JVector3& axis, float angle)
{
	JMatrix m;
	m.SetSIMD(DirectX::XMMatrixRotationAxis(JVector3::GetSIMD(axis), angle));
	return m;
}
JMatrix JMatrix::Scaling(const JVector3& v)
{
	JMatrix m;
	m.SetSIMD(DirectX::XMMatrixScaling(v.x, v.y, v.z));
	return m;
}
JMatrix JMatrix::LookAtLH(const JVector3& pos, const JVector3& target, const JVector3& up)
{
	JMatrix m;
	m.SetSIMD(DirectX::XMMatrixLookAtLH(
		JVector3::GetSIMD(pos),
		JVector3::GetSIMD(target),
		JVector3::GetSIMD(up)));
	return m;
}