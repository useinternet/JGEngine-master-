#pragma once
#include"../EngineStatics/Engine.h"
// D3D11_VIEWPORT
// float m_fFov;
// float m_fFarZ;
// float m_fNearZ;
// ���� worldMatrix, projectionMatrix, orthMatrix

class JGMatrix;


/*
Class : JGViewportD 
@m unique_ptr<D3D11_VIEWPORT> m_Viewport   : D3D11_VIEWPORT ����ü 
@m unique_ptr<JGMatrix> m_projectionMatrix : ���� ��� 
@m unique_ptr<JGMatrix> m_orthoMatrix      : ���翵 ��� 
@m float m_FOV    : FieldOfView (�þ߰�) 
@m float m_FarZ   : ������ �ִ� �Ÿ�
@m float m_NearZ  : ������ �ִ� �Ÿ� 
@m float m_Width  : ����Ʈ ���α���
@m float m_Height : ����Ʈ ���α��� */
class ENGINE_EXPORT JGViewportD
{
private:
	std::unique_ptr<D3D11_VIEWPORT> m_Viewport;
	std::unique_ptr<JGMatrix>       m_projectionMatrix;
	std::unique_ptr<JGMatrix>       m_orthoMatrix;
	float m_FOV    = 0.0f;
	float m_FarZ   = 0.0f;
	float m_NearZ  = 0.0f;
	static float m_Width;
	static float m_Height;
public:
	JGViewportD();
	~JGViewportD();
	/*
	Exp : ����Ʈ�� �ʱ�ȭ �Ѵ�. 
	@param const int Width   : ���� ����
	@param const int Height  : ���� ����
	@param const float FOV   : �þ߰�
	@param const float FarZ  : ������ �ִ� �Ÿ�
	@param const float NearZ : ������ �ּ� �Ÿ�  */
	bool InitViewport(const int Width, const int Height, const float FOV, const float FarZ, const float NearZ);

	/*
	Exp : ����Ʈ�� �����´�. */
	D3D11_VIEWPORT* Get();
	/*
	Exp : ���� ����� �����´�. */
	JGMatrix& GetProjectionMatrix();
	/*
	Exp : ���翵 ����� �����´�. */
	JGMatrix& GetOrthoMatrix();

	/*
	Exp : �þ߰��� �����´�. */
	const float GetFOV();
	/*
	Exp : ���α��̸� �����´�. */
	static const float GetWidth();
	/*
	Exp : ���α��̸� �����´�. */
	static const float GetHeight();

	/*
	Exp : �þ߰��� �����Ѵ�. */
	void SetFOV(const float FOV);
private:
	void CreateViewport(const float MinDepth = 0.0f, const float MaxDepth = 1.0f, 
		const float TopLeftX = 0.0f, const float TopLeftY = 0.0f);
	void CreateProjectionMatrix();
	void CreateOrthoMatrix();
};