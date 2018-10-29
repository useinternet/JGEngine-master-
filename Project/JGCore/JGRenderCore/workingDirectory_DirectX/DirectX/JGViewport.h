#pragma once
#include"Common/JGRCCommon.h"
#include"DirectXCommon.h"

namespace JGRC
{
	class CORE_EXPORT JGViewport
	{
	private:
		std::unique_ptr<D3D11_VIEWPORT> m_Viewport;
		std::unique_ptr<jgMatrix4x4> m_projectionMatrix;
		std::unique_ptr<jgMatrix4x4> m_orthoMatrix;
		real m_FOV = 0;
		real m_FarZ = 0;
		real m_NearZ = 0;
		real m_Width = 0;
		real m_Height = 0;
	public:
		JGViewport();
		~JGViewport();
		/*
		Exp : ����Ʈ�� �ʱ�ȭ �Ѵ�.
		@param const int Width   : ���� ����
		@param const int Height  : ���� ����
		@param const float FOV   : �þ߰�
		@param const float FarZ  : ������ �ִ� �Ÿ�
		@param const float NearZ : ������ �ּ� �Ÿ�  */
		bool InitViewport(const int Width, const int Height, const real FOV, const real FarZ, const real NearZ);

		/*
		Exp : ����Ʈ�� �����´�. */
		D3D11_VIEWPORT* Get();
		///*
		//Exp : ���� ����� �����´�. */
		jgMatrix4x4& GetProjectionMatrix();
		///*
		//Exp : ���翵 ����� �����´�. */
		jgMatrix4x4& GetOrthoMatrix();

		/*
		Exp : �þ߰��� �����´�. */
		const real GetFOV();
		/*
		Exp : ���α��̸� �����´�. */
		const real GetWidth();
		/*
		Exp : ���α��̸� �����´�. */
		const real GetHeight();

		/*
		Exp : �þ߰��� �����Ѵ�. */
		void SetFOV(const real FOV);
	private:
		void CreateViewport(const real MinDepth = 0.0f, const real MaxDepth = 1.0f,
			const real TopLeftX = 0.0f, const real TopLeftY = 0.0f);
		void CreateProjectionMatrix();
		void CreateOrthoMatrix();
	};


}

