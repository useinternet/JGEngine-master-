#include"JGViewportD.h"
using namespace std;
JGViewportD::JGViewportD()
{
	m_Viewport = make_unique<D3D11_VIEWPORT>();
	m_projectionMatrix = make_unique<D3DXMATRIX>();
	m_orthoMatrix = make_unique<D3DXMATRIX>();
}
JGViewportD::~JGViewportD()
{

}
bool JGViewportD::InitViewport(const int Width, const int Height, const float FOV, const float FarZ, const float NearZ)
{
	m_Width  = (float)Width;
	m_Height = (float)Height;
	m_FOV    = FOV;
	m_FarZ   = FarZ;
	m_NearZ  = NearZ;


	// ����Ʈ ����
	CreateViewport();
	// ���� ��� ����
	CreateProjectionMatrix();
	// ���翵 ��� ����
	CreateOrthoMatrix();

	return true;
}
D3D11_VIEWPORT * JGViewportD::Get()
{
	return m_Viewport.get();
}
void JGViewportD::GetProjectionMatrix(D3DXMATRIX* projectionMatrix)
{
	*projectionMatrix = *m_projectionMatrix;
}

void JGViewportD::GetOrthoMatrix(D3DXMATRIX* OrthoMatrix)
{
	*OrthoMatrix = *m_orthoMatrix;
}

const float JGViewportD::GetFOV()
{
	return m_FOV;
}

void JGViewportD::SetFOV(const float FOV)
{
	m_FOV = FOV;
	CreateProjectionMatrix();
}
void JGViewportD::CreateViewport(const float MinDepth, const float MaxDepth, const float TopLeftX, const float TopLeftY)
{
	m_Viewport->Width = m_Width;
	m_Viewport->Height = m_Height;
	m_Viewport->MinDepth = MinDepth;
	m_Viewport->MaxDepth = MaxDepth;
	m_Viewport->TopLeftX = TopLeftX;
	m_Viewport->TopLeftY = TopLeftY;
}
void JGViewportD::CreateProjectionMatrix()
{
	// ȭ�� ������ ���Ѵ�.
	float ScreenAspect = m_Width / m_Height;
	// ���� ����� �����.
	D3DXMatrixPerspectiveFovLH(m_projectionMatrix.get(), ToRadian(m_FOV), ScreenAspect, m_NearZ, m_FarZ);
}
void JGViewportD::CreateOrthoMatrix()
{
	// 2D �������� ���� ���翵 ����� �����մϴ�.
	D3DXMatrixOrthoLH(m_orthoMatrix.get(), m_Width, m_Height, m_NearZ, m_FarZ);
}
